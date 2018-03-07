#-------------------------------------------------
#
# Project created by QtCreator 2017-02-14T16:42:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SaveMobame
TEMPLATE = app


SOURCES += main.cpp\
        savemobame.cpp \
    image.cpp

HEADERS  += savemobame.h \
    image.h

FORMS    += savemobame.ui \
    image.ui

RESOURCES += \
    src.qrc

RC_FILE = main.rc
