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
        if(!img.empty())
        {
            std::cout<<"Image loaded to opencv!\n";
            auto processed=OilEffect2(img, ui->radiusSlider->value() ,ui->intensitySlider->value());
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


