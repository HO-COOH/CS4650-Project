#ifndef FRAME_H
#define FRAME_H

#include <QDialog>
#include <QAbstractButton>
#include <opencv2/opencv.hpp>

namespace Ui {
class frame;
}

class frame : public QDialog
{
    Q_OBJECT

public:
    explicit frame(QWidget *parent = nullptr);
    ~frame();

private slots:


    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::frame *ui;
};

struct frame_size
{
    size_t up;
    size_t down;
    size_t left;
    size_t right;
};
frame_size get_frame_size(const cv::Mat& frame_img);

#endif // FRAME_H
