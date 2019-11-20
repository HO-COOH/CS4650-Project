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

void Widget::on_radiusSlider_sliderMoved(int position)
{
    ui->radiusText->setText(QString::number(position));
}

void Widget::on_intensitySlider_sliderMoved(int position)
{
    ui->intensityText->setText(QString::number(position));
}

void Widget::on_applyBtn_clicked()
{
    if(!imageLoaded)
        QMessageBox::warning(this, "Error", "No image is loaded!");
    else
    {
        cv::namedWindow("img");
        auto img=cv::imread(fileName.toStdString());
        if(!img.empty())
        {
            std::cout<<"Image loaded to opencv!\n";
            cv::imshow("img",OilEffect2(img,1,10));
            cv::waitKey(0);
            cv::destroyAllWindows();
        }
        else
        {
            std::cout<<"Crap happened!\n";
        }
    }
}
