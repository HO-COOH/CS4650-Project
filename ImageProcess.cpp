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
