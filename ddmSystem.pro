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
    ddmCounty.cpp \
    ddmBoundary.cpp \
    ddmObject.cpp \
    ddmModel.cpp \
    ddmState.cpp \
    ddmMapView.cpp \
    ddmWidget.cpp \
    ddmMapViewPage.cpp \
    ddmTableViewModel.cpp \
    ddmTableView.cpp \
    ddmEmptyFilterWidget.cpp \
    ddmFilter.cpp \
    ddmEmptyFilter.cpp \
    ddmFrictionCountyFilter.cpp \
    ddmFrictionCountyFilterWidget.cpp

HEADERS  += mainwindow.h \
    ddmCounty.h \
    ddmBoundary.h \
    ddmObject.h \
    ddmModel.h \
    ddmState.h \
    ddmMapView.h \
    ddmWidget.h \
    ddmMapViewPage.h \
    ddmTableViewModel.h \
    ddmTableView.h \
    ddmEmptyFlterWidget.h \
    ddmEmptyFilter.h \
    ddmFilter.h \
    ddmVersion.h \
    ddmFrictionCountyFilter.h \
    ddmFrictionCountyFilterWidget.h

FORMS    += mainwindow.ui \
    ddmwidget.ui \
    ddmtableview.ui \
    ddmEmptyFilterWidget.ui \
    ddmfrictioncountyfilterwidget.ui

Release:DESTDIR = release
Debug:DESTDIR = debug

RESOURCES += \
    icons.qrc

