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
    /*set default values*/
    ui->radiusSlider->setRange(1,5);
    ui->radiusSlider->setValue(1);
    ui->intensitySlider->setValue(20);
    ui->gaussianSlider->setValue(11);
    ui->sigmaSlider->setValue(5);
    ui->edgeSlider->setValue(100);

    ui->chromaticCheckbox->setCheckState(Qt::CheckState::Checked);
    ui->monochromeCheckbox->setCheckState(Qt::CheckState::Unchecked);

    ui->nextBtn->setEnabled(false);
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
        original=cv::imread(fileName.toStdString());

        //check if image is black and white
        cv::Mat test[3];
        isGrayScale = false;
        cv::split(original, test);
        for(int i = 0; i < original.rows; ++i)
        {
            for(int j = 0; j < original.cols; ++j)
            {
                if(test[0].at<uint8_t>(i,j) == test[1].at<uint8_t>(i,j) && test[0].at<uint8_t>(i,j) == test[2].at<uint8_t>(i,j))
                    isGrayScale = true;
                else
                {
                    isGrayScale = false;
                    break;
                }
            }
        }

        //if is grayScale, disable chromatic group
        if(isGrayScale)
        {
            ui->radiusText->setEnabled(false);
            ui->radiusSlider->setEnabled(false);
            ui->intensityText->setEnabled(false);
            ui->intensitySlider->setEnabled(false);
            ui->monochromeCheckbox->setCheckState(Qt::CheckState::Checked);
            ui->chromaticCheckbox->setCheckState(Qt::CheckState::Unchecked);
        }
        else    //not grayscale, disable monochrome group
        {
            ui->chromaticCheckbox->setCheckState(Qt::CheckState::Checked);
            ui->chromaticCheckbox->setCheckState(Qt::CheckState::Unchecked);
        }
    }
    else //image load unsucessfully
        QMessageBox::warning(this, "Error", "This file is not an image!");


}

void Widget::on_radiusSlider_valueChanged(int value)
{
    ui->radiusText->setText(QString::number(value));
    if(imageLoaded)
        Draw();
}

void Widget::on_intensitySlider_valueChanged(int value)
{
    ui->intensityText->setText(QString::number(value));
    if(imageLoaded)
        Draw();
}

void Widget::on_applyBtn_clicked()
{
    if(imageLoaded)
        Draw();
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
    frame_window=new frame(this, processed, isGrayScale);
    frame_window->setModal(true);
    frame_window->show();
}

void Widget::on_edgeSlider_valueChanged(int value)
{
    ui->edgeText->setText(QString::number(value));
    if(imageLoaded)
        Draw();
}

void Widget::Draw()
{
    std::cout<<"Image loaded to opencv!\n";
    if(isGrayScale)
    {
        std::cout << "It's black and white";
        cv::Mat splited[3];
        cv::split(original, splited);
        processed = PencilEffect(splited[0],ui->gaussianSlider->value(),ui->sigmaSlider->value());
        ui->imageLabel->setPixmap(QPixmap::fromImage(QImage((unsigned char*)processed.data, processed.cols, processed.rows, processed.step, QImage::Format_Grayscale8)));
    }
    else
    {
        processed=OilEffect2(original, ui->radiusSlider->value() ,ui->intensitySlider->value());
        processed=CartoonEffect(processed,ui->gaussianSlider->value(),ui->sigmaSlider->value(),ui->edgeSlider->value());
        ui->imageLabel->setPixmap(QPixmap::fromImage(QImage((unsigned char*)processed.data, processed.cols, processed.rows, processed.step, QImage::Format_RGB888).rgbSwapped()));
    }
    ui->nextBtn->setEnabled(true);
}

void Widget::on_gaussianSlider_valueChanged(int value)
{
    if(value%2==0)
    {
        value+=1;
        ui->gaussianSlider->setValue(value);
    }
    ui->gaussianText->setText(QString::number(value));
    if(imageLoaded)
        Draw();
}



void Widget::on_sigmaSlider_valueChanged(int value)
{
    ui->sigmaText->setText(QString::number(value));
    if(imageLoaded)
        Draw();
}
