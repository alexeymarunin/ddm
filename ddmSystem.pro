#-------------------------------------------------
#
# Project created by QtCreator 2014-11-29T13:23:51
#
#-------------------------------------------------

QT       += core gui webkit network
QT       += webkitwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ddmSystem
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ddmmapswidget.cpp \
    ddmObjectModel.cpp

HEADERS  += mainwindow.h \
    ddmmapswidget.h \
    ddmObjectModel.h

FORMS    += mainwindow.ui \
    ddmmapswidget.ui
