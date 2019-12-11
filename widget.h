#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "frame.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_loadBtn_clicked();

    void on_applyBtn_clicked();

    void on_radiusSlider_valueChanged(int value);

    void on_intensitySlider_valueChanged(int value);


    void on_monochromeCheckbox_stateChanged(int arg1);

    void on_chromaticCheckbox_stateChanged(int arg1);

    void on_nextBtn_clicked();

    void on_edgeSlider_valueChanged(int value);

    void on_gaussianSlider_valueChanged(int value);

    void on_sigmaSlider_valueChanged(int value);

private:
    Ui::Widget *ui;
    cv::Mat original;
    cv::Mat processed;
    bool isGrayScale;
    frame* frame_window;
    void Draw();
};

#endif // WIDGET_H
