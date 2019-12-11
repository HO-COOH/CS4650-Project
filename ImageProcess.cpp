#include "ImageProcess.h"
#include <vector>
#include <algorithm>
#include <numeric>
using namespace cv;
using namespace std;

cv::Mat OilEffect(const cv::Mat& original, int radius, int intensity)
{
	Mat processed;
	copyMakeBorder(original, processed, radius, radius, radius, radius, cv::BORDER_REFLECT);	//padding the image
	vector<cv::Vec3b> neighbors(radius * radius);
	
	for (auto row = radius; row != original.rows+radius; ++row)
	{
		for (auto col = radius; col != original.cols+radius; ++col)
		{
			//push the neighbor (radius*radius) pixels to the vector
			size_t index = 0;
			for (auto x = row - radius; x != row + radius+1; ++x)
			{
				for (auto y = col - radius; y != col + radius + 1; ++y)
					neighbors[index++]=processed.at<Vec3b>(x, y);
			}
			//calculate levels
			vector<pair<unsigned char, Vec3i>> intensity_count(intensity);	//(count, RGB)
			for_each(neighbors.begin(), neighbors.end(), [&](const cv::Vec3b& pixel) {
				int r = pixel[0], g = pixel[1], b = pixel[2];
				unsigned char count = (r + g + b) / 3.0 * intensity / 255;
				++intensity_count[count].first;
				intensity_count[count].second[0] += r;
				intensity_count[count].second[1] += g;
				intensity_count[count].second[2] += b;
				});
			auto intensity_most = std::max_element(intensity_count.begin(), intensity_count.end(), [](const std::pair<unsigned char, Vec3i>& l, const std::pair<unsigned char, Vec3i>& r) {return l.first < r.first; });
			intensity_most->second[0] /= intensity_most->first;
			intensity_most->second[1] /= intensity_most->first;
			intensity_most->second[2] /= intensity_most->first;
			processed.at<Vec3b>(row, col) = (intensity_most->second[0], intensity_most->second, intensity_most->second);
		}
	}
	return processed;
}

cv::Mat OilEffect2(const cv::Mat& original, int radius, int intensity)
{
    Mat processed;
    copyMakeBorder(original, processed, radius, radius, radius, radius, cv::BORDER_REFLECT);	//padding the image
    vector<pair<unsigned char, Vec3i>> intensity_count(intensity+1);
    for (auto row = radius; row != original.rows + radius; ++row)
    {
        for (auto col = radius; col != original.cols + radius; ++col)
        {
            std::fill(intensity_count.begin(), intensity_count.end(), make_pair(0,Vec3i{0,0,0}));
            //push the neighbor (radius*radius) pixels to the vector
                //(count, RGB)
            for (auto x = row - radius; x != row + radius + 1; ++x)
            {
                for (auto y = col - radius; y != col + radius + 1; ++y)
                {
                    auto& pixel = processed.at<Vec3b>(x, y);
                    int r = pixel[0], g = pixel[1], b = pixel[2];
                    unsigned char count = (r + g + b) / 3.0 * intensity / 255;
                    ++intensity_count[count].first;
                    intensity_count[count].second[0] += r;
                    intensity_count[count].second[1] += g;
                    intensity_count[count].second[2] += b;
                }
            }
            auto intensity_most = std::max_element(intensity_count.begin(), intensity_count.end(), [](const auto& l, const auto& r) {return l.first < r.first; });
            intensity_most->second[0] /= intensity_most->first;
            intensity_most->second[1] /= intensity_most->first;
            intensity_most->second[2] /= intensity_most->first;
            processed.at<Vec3b>(row, col) = (intensity_most->second[0], intensity_most->second, intensity_most->second);
        }
    }
    return processed;
}


Mat PencilEffect(const Mat& original, int K_SIZE, int S_SIZE) {
    original.convertTo(original, CV_8UC1);

    int row = original.rows;
    int col = original.cols;

    Mat img = Mat::zeros(row,col,CV_8UC1);
    Mat img_invert = Mat::zeros(row,col,CV_8UC1);
    Mat img_pencil = Mat::zeros(row,col,CV_8UC1);

    //create a clone of the original image
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            img.at<uint8_t>(i,j) = original.at<uint8_t>(i,j);
        }
    }

    //create an invert image
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            img_invert.at<uint8_t>(i,j) = 255 - img.at<uint8_t>(i,j);
        }
    }



    img_invert.convertTo(img_invert, CV_32FC1);
    img_pencil.convertTo(img_pencil, CV_32FC1);
    img.convertTo(img, CV_32FC1);


    //blur the inverted image with Gaussian filter
    GaussianBlur(img_invert, img_invert, Size(K_SIZE,K_SIZE), S_SIZE);


    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            //blending with white give white
            if(img_invert.at<float>(i,j) == 255){
                img_pencil.at<float>(i,j) = 255;
            //blending with black doesn't change the color
            }else if(img_invert.at<float>(i,j) == 0){
                img_pencil.at<float>(i,j) = img.at<float>(i,j);
            //divide gray scale image by the top inverted image
            }else{
                img_pencil.at<float>(i,j) = img.at<float>(i,j)*255 / (255-img_invert.at<float>(i,j));
            }
        }
    }


    img_pencil.convertTo(img_pencil, CV_8UC1);
    img_invert.convertTo(img_invert, CV_8UC1);
    img.convertTo(img, CV_8UC1);

    //Apply median filter to clean up noise in drawing
    medianBlur(img_pencil, img_pencil, (K_SIZE / 2)%2==0? K_SIZE/2+1 :K_SIZE/2 );

    return img_pencil;
}

Mat CartoonEffect(const Mat& img, int K_SIZE, int S_SIZE, int T_SIZE) {
       int row = img.rows;
       int col = img.cols;

       Mat img_painting = img.clone();
       Mat img_edge = Mat::zeros(row,col,CV_8UC1);
       Mat img_blur = Mat::zeros(row, col, CV_8UC1);
       Mat img_clone = Mat::zeros(row, col, CV_8UC1);



       cvtColor(img_painting, img_clone, COLOR_BGR2GRAY); //convert image to gray scale

       //enhance the edge of the image using unsharp masking
       GaussianBlur(img_clone, img_blur, Size(K_SIZE,K_SIZE), S_SIZE);
       Mat img_sub_mod = img_clone - img_blur; //input - blur image
       Mat img_sharp_mod = img_clone + img_sub_mod; //input + (input - blur image)

       //Find thresholds for Canny edge detection
       double h_thresh = T_SIZE, l_thresh = 0;
   //    int count = 0;
   //
   //    for(int i = 0; i < row; i++){
   //        for(int j = 0; j < col; j++){
   //            if(img_painting.at<uint8_t>(i,j) != 0){
   //                h_thresh += img_painting.at<uint8_t>(i,j);
   //                count++;
   //            }
   //        }
   //    }

   //    h_thresh /= count;
       l_thresh = h_thresh/3;
       Canny(img_sharp_mod, img_edge, h_thresh, l_thresh);
       //Laplacian(img_sharp_mod, img_edge, 1);
   //    Mat img_blur;
   //    medianBlur(img, img_blur, 7);
   //    adaptiveThreshold(img_blur, img_edge, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 9, 2);


       for(int i = 0; i < row; i++){
           for(int j = 0; j < col; j++){
               if(img_edge.at<uint8_t>(i,j) == 255){
                   Vec3b color = {0,0,0};
                   img_painting.at<Vec3b>(i,j) = color;
               }
           }
       }

       return img_painting;

}
