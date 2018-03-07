#ifndef IMAGE_H
#define IMAGE_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class Image;
}

class Image : public QWidget
{
    Q_OBJECT

public:
    explicit Image(QWidget *parent = 0);
    ~Image();

private slots:
    void on_setBt_clicked();

    void on_resetBt_clicked();

private:
    Ui::Image *ui;
};

#endif // IMAGE_H
