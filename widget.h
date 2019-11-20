#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

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

    void on_radiusSlider_sliderMoved(int position);

    void on_intensitySlider_sliderMoved(int position);

    void on_applyBtn_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
