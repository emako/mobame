#include "image.h"
#include "ui_image.h"

Image::Image(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Image)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Image"));
    this->setWindowOpacity(0.99);

    QSettings *configIniRead = new QSettings("configure.ini", QSettings::IniFormat);
    QString imageStr = configIniRead->value("/SaveMobame/image").toString();
    ui->imagenameEdit->setText(imageStr);
    delete configIniRead;
}

Image::~Image()
{
    delete ui;
}

void Image::on_setBt_clicked()
{
    QSettings *configIniWrite = new QSettings("configure.ini", QSettings::IniFormat);
    configIniWrite->setValue("/SaveMobame/image", ui->imagenameEdit->text());
    delete configIniWrite;
    this->close();
}

void Image::on_resetBt_clicked()
{
    QSettings *configIniWrite = new QSettings("configure.ini", QSettings::IniFormat);
    configIniWrite->setValue("/SaveMobame/image", "img2");
    ui->imagenameEdit->setText("img2");
    delete configIniWrite;
}
