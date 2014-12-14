#-------------------------------------------------
#
# Project created by QtCreator 2014-11-29T13:23:51
#
#-------------------------------------------------

QT       += core gui webkit network
QT       += webkitwidgets
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ddmSystem
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ddmObjectModel.cpp \
    ddmMap.cpp \
    ddmMapsWidget.cpp

HEADERS  += mainwindow.h \
    ddmObjectModel.h \
    ddmMap.h \
    ddmMapsWidget.h

FORMS    += mainwindow.ui \
    ddmmapswidget.ui

DESTDIR = $$PWD

include(xlsx/qtxlsx.pri)
