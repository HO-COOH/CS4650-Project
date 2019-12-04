#include "frame.h"
#include "ui_frame.h"
#include <QLabel>
#include <QPixmap>
#include <QScrollArea>

frame::frame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frame)
{
    ui->setupUi(this);

    /*Showing all image with frame*/
    QLabel* label=new QLabel();
    QPixmap pic(":/frames/1.png");
    label->setPixmap(pic);
    ui->scrollArea->setWidget(label);
    //ui->scrollArea->setLayout(new QVBoxLayout());
    ui->label->setPixmap(QPixmap(":/frames/1.png"));

    //connect(ui->label, "click", this, )

}

frame::~frame()
{
    delete ui;
}

frame_size get_frame_size(const cv::Mat& frame_img)
{
    using namespace cv;
    //sample a frame image at its center to get the color info
//    auto width=frame_img.cols;
//    auto height=frame_img.rows;
    uchar center=frame_img.at<uchar>(frame_img.rows/2, frame_img.cols/2);
    size_t up=0, bottom=0, left=0, right=0;
    //start from the center, scan from left -> right
    for(size_t x=0; x!=frame_img.cols; ++x)
    {
        if(frame_img.at<uchar>(frame_img.rows/2, x)==center)
        {
            left=x;
            break;
        }
    }
    for(size_t x=frame_img.cols; x!=0; --x)
    {
        if(frame_img.at<uchar>(frame_img.rows/2, x)==center)
        {
            right=x;
            break;
        }
    }
    for(size_t y=0; y!=frame_img.rows; ++y)
    {
        if(frame_img.at<uchar>(y, frame_img.cols/2)==center)
        {
            up=y;
            break;
        }
    }
    for(size_t y=frame_img.rows; y!=0; --y)
    {
        if(frame_img.at<uchar>(y, frame_img.cols/2)==center)
        {
            bottom=y;
            break;
        }
    }
    return {up, bottom, left, right};
}


void frame::on_okBtn_clicked()
{

}

void frame::on_cancelBtn_clicked()
{
    this->close();
}
