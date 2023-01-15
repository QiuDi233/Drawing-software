#-------------------------------------------------
#
# Project created by QtCreator 2023-01-11T23:23:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VCLPainting
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    figure.cpp \
    utils.cpp

HEADERS  += mainwindow.h \
    figure.h \
    utils.h

FORMS    += mainwindow.ui

RESOURCES += \
    src.qrc

DISTFILES +=
