#include "savemobame.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_Use96Dpi);
    QApplication a(argc, argv);
    SaveMobame w;
    w.show();

    return a.exec();
}
