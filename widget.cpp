#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QtDebug>
#include <QPixmap>
#include <QMessageBox>
#include "ImageProcess.h"


static bool imageLoaded=false;
static QString fileName;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->imageLabel->setText("");
    ui->radiusSlider->setRange(1,5);
    ui->radiusSlider->setValue(1);

    ui->intensitySlider->setValue(20);

    ui->chromaticCheckbox->setCheckState(Qt::CheckState::Checked);
    ui->monochromeCheckbox->setCheckState(Qt::CheckState::Unchecked);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_loadBtn_clicked()
{
    fileName=QFileDialog::getOpenFileName(this, "Choose an image", {});
    qDebug()<<fileName<<"\n";   //debug
    QPixmap img(fileName);
    if(!img.isNull())
    {
        img=img.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio);
        ui->imageLabel->setPixmap(img);
        imageLoaded=true;
    }
    else //image load unsucessfully
        QMessageBox::warning(this, "Error", "This file is not an image!");
}

void Widget::on_radiusSlider_valueChanged(int value)
{
    ui->radiusText->setText(QString::number(value));
}

void Widget::on_intensitySlider_valueChanged(int value)
{
    ui->intensityText->setText(QString::number(value));
}

void Widget::on_applyBtn_clicked()
{
    if(!imageLoaded)
        QMessageBox::warning(this, "Error", "No image is loaded!");
    else
    {
        cv::namedWindow("img");
        auto img=cv::imread(fileName.toStdString());
        auto processed = img.clone();

        //check if image is black and white
        cv::Mat test[3];
        bool isGrayScale = false;
        cv::split(img, test);
        for(int i = 0; i < img.rows; i++){
            for(int j = 0; j < img.cols; j++){
                if(test[0].at<uint8_t>(i,j) == test[1].at<uint8_t>(i,j) && test[0].at<uint8_t>(i,j) == test[2].at<uint8_t>(i,j)){
                    isGrayScale = true;
                }else{
                    isGrayScale = false;
                }
            }
        }


        if(!img.empty())
        {
            std::cout<<"Image loaded to opencv!\n";
            if(isGrayScale){
                std::cout << "It's black and white";
                processed = PencilEffect(test[0],11,5);
            }else{
                processed=OilEffect2(img, ui->radiusSlider->value() ,ui->intensitySlider->value());
            }
            cv::imshow("img",processed);
            cv::waitKey(0);
            cv::destroyAllWindows();
        }
        else
        {
            std::cout<<"Crap happened!\n";
        }
    }
}


void Widget::on_monochromeCheckbox_stateChanged(int arg1)
{
    ui->chromaticCheckbox->setCheckState(arg1==Qt::CheckState::Checked? Qt::CheckState::Unchecked : Qt::CheckState::Checked);
}

void Widget::on_chromaticCheckbox_stateChanged(int arg1)
{
    ui->monochromeCheckbox->setCheckState(arg1==Qt::CheckState::Checked? Qt::CheckState::Unchecked : Qt::CheckState::Checked);
}

void Widget::on_nextBtn_clicked()
{
    //hide();
    frame_window=new frame(this);
    frame_window->setModal(true);
    frame_window->show();
}
