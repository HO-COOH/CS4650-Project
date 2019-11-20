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

    void on_applyBtn_clicked();

    void on_radiusSlider_valueChanged(int value);

    void on_intensitySlider_valueChanged(int value);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
