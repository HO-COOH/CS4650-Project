#include "frame.h"
#include "ui_frame.h"
#include <QLabel>
#include <QPixmap>
#include <iostream>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>

frame::frame(QWidget *parent, cv::Mat& processed, bool isGrayScale) :
    QDialog(parent),
    ui(new Ui::frame)
{
    ui->setupUi(this);
    do
    {
        fileName=QFileDialog::getOpenFileName(this, "Choose an image", {});
        if(!fileName.isEmpty())
            break;
        QMessageBox::critical(this, "Error", "Choose a frame!");
    }while(true);
    frame_img=cv::imread(fileName.toStdString());
    auto frame_size=get_frame_size(cv::imread(fileName.toStdString(),cv::IMREAD_GRAYSCALE));

    /*Debug*/
    print_size(frame_size);

    add_frame(processed, frame_img, frame_size, isGrayScale);
    ui->label->setPixmap(QPixmap::fromImage(QImage((unsigned char*)frame_img.data, frame_img.cols, frame_img.rows, frame_img.step, QImage::Format_RGB888).rgbSwapped()));
}

frame::~frame()
{
    delete ui;
}

frame_size get_frame_size(const cv::Mat& frame_img)
{
    using namespace cv;
    //sample a frame image at its center to get the color info
    uchar center = frame_img.at<uchar>(frame_img.rows / 2, frame_img.cols / 2);
    size_t up = 0, bottom = 0, left = 0, right = 0;
    //start from the center, scan from left -> right
    for (size_t x = 0; x != frame_img.cols; ++x)
    {
        if (frame_img.at<uchar>(frame_img.rows / 2, x) == center && frame_img.at<uchar>(frame_img.rows/2, x+2)==center)
        {
            left = x;
            break;
        }
    }
    for (size_t x = frame_img.cols-1; x != 0; --x)
    {
        if (frame_img.at<uchar>(frame_img.rows / 2, x) == center && frame_img.at<uchar>(frame_img.rows/2, x-2)==center)
        {
            right = x;
            break;
        }
    }
    for (size_t y = 0; y != frame_img.rows; ++y)
    {
        if (frame_img.at<uchar>(y, frame_img.cols / 2) == center)
        {
            up = y;
            break;
        }
    }
    for (size_t y = frame_img.rows-1; y != 0; --y)
    {
        if (frame_img.at<uchar>(y, frame_img.cols / 2) == center)
        {
            bottom = y;
            break;
        }
    }
    return { up, bottom, left, right };
}


void frame::on_okBtn_clicked()
{
    cv::imwrite("final.jpg", frame_img);
}

void frame::on_cancelBtn_clicked()
{
    this->close();
}

void add_frame(cv::Mat& img,cv::Mat& frame, const frame_size frameSize, bool isGrayScale)
{
    using namespace cv;
    cv::resize(img, img, cv::Size(frameSize.right-frameSize.left, frameSize.down-frameSize.up));
    if(isGrayScale)
    {
        for(size_t row=frameSize.up, origin_row=0; row<frameSize.down; ++row, ++origin_row)
        {
            for(size_t col=frameSize.left, origin_col=0; col<frameSize.right; ++col, ++origin_col)
                frame.at<Vec3b>(row, col)={ img.at<uchar>(origin_row, origin_col), img.at<uchar>(origin_row, origin_col), img.at<uchar>(origin_row, origin_col)};
        }
    }
    else
    {
        for(size_t row=frameSize.up, origin_row=0; row<frameSize.down; ++row, ++origin_row)
        {
            for(size_t col=frameSize.left, origin_col=0; col<frameSize.right; ++col, ++origin_col)
                frame.at<Vec3b>(row, col)=img.at<Vec3b>(origin_row, origin_col);
        }
    }
    namedWindow("test");
    imshow("test", frame);
    waitKey(0);
}

void print_size(const frame_size frameSize)
{
    std::cout<<"up="<<frameSize.up<<", down="<<frameSize.down<<", left="<<frameSize.left<<", right="<<frameSize.right<<"\n";
}
