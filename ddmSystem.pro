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
LANGUAGE = C++
CONFIG += console

SOURCES += main.cpp \
    base/ddmObject.cpp \
    base/ddmState.cpp \
    base/ddmBoundary.cpp \
    base/ddmCounty.cpp \
    base/ddmDatabase.cpp \
    models/ddmModel.cpp \
    models/ddmStateModel.cpp \
    models/ddmFilterModel.cpp \
    models/ddmCountyFilterModel.cpp \
    filters/ddmFilter.cpp \
    filters/ddmCountyFilter.cpp \
    widgets/ddmFilterWidget.cpp \
    widgets/ddmCountyFilterWidget.cpp \
    widgets/ddmCentralWidget.cpp \
    ddmMapView.cpp \
    ddmMapViewPage.cpp \
    ddmMainWindow.cpp \
    base/ddmPoint.cpp \
    base/ddmCenterPoint.cpp \
    base/ddmMapObject.cpp \
    ddmApplication.cpp \
    filters/ddmEmptyFilter.cpp \
    widgets/ddmEmptyFilterWidget.cpp \
    models/ddmEmptyFilterModel.cpp \
    models/ddmMinMaxFrictionFilterModel.cpp \
    widgets/ddmMinMaxFrictionFilterWidget.cpp \
    filters/ddmMinMaxFrictionFilter.cpp \
    models/ddmPosNegDeltaFilterModel.cpp \
    widgets/ddmPosNegDeltaFilterWidget.cpp \
    filters/ddmPosNegDeltaFilter.cpp \
    models/ddmFrictionDeltaFilterModel.cpp \
    filters/ddmFrictionDeltaFilter.cpp \
    widgets/ddmFrictionDeltaFilterWidget.cpp \
    filters/ddmFrictionPopulationFilter.cpp \
    models/ddmFrictionPopulationFilterModel.cpp \
    widgets/ddmFrictionPopulationFilterWidget.cpp \
    widgets/ddmDeltaPopulationFilterWidget.cpp \
    models/ddmDeltaPopulationFilterModel.cpp \
    filters/ddmDeltaPopulationFilter.cpp

HEADERS += \
    ddmVersion.h \
    base/ddmObject.h \
    base/ddmState.h \
    base/ddmBoundary.h \
    base/ddmCounty.h \
    base/ddmDatabase.h \
    models/ddmModel.h \
    models/ddmStateModel.h \
    models/ddmFilterModel.h \
    models/ddmCountyFilterModel.h \
    filters/ddmFilter.h \
    filters/ddmCountyFilter.h \
    widgets/ddmFilterWidget.h \
    widgets/ddmCountyFilterWidget.h \
    widgets/ddmCentralWidget.h \
    ddmMapView.h \
    ddmMapViewPage.h \
    ddmMainWindow.h \
    base/ddmPoint.h \
    base/ddmCenterPoint.h \
    base/ddmMapObject.h \
    ddmApplication.h \
    filters/ddmEmptyFilter.h \
    widgets/ddmEmptyFilterWidget.h \
    models/ddmEmptyFilterModel.h \
    models/ddmMinMaxFrictionFilterModel.h \
    widgets/ddmMinMaxFrictionFilterWidget.h \
    filters/ddmMinMaxFrictionFilter.h \
    models/ddmPosNegDeltaFilterModel.h \
    widgets/ddmPosNegDeltaFilterWidget.h \
    filters/ddmPosNegDeltaFilter.h \
    filters/ddmFrictionDeltaFilter.h \
    models/ddmFrictionDeltaFilterModel.h \
    widgets/ddmFrictionDeltaFilterWidget.h \
    base/ddmDefines.h \
    filters/ddmFrictionPopulationFilter.h \
    models/ddmFrictionPopulationFilterModel.h \
    widgets/ddmFrictionPopulationFilterWidget.h \
    widgets/ddmDeltaPopulationFilterWidget.h \
    models/ddmDeltaPopulationFilterModel.h \
    filters/ddmDeltaPopulationFilter.h

FORMS += ddmMainWindow.ui \
    widgets/ddmCountyFilterWidget.ui \
    widgets/ddmCentralWidget.ui \
    widgets/ddmEmptyFilterWidget.ui \
    widgets/ddmMinMaxFrictionFilterWidget.ui \
    widgets/ddmPosNegDeltaFilterWidget.ui \
    widgets/ddmFrictionDeltaFilterWidget.ui \
    widgets/ddmFrictionPopulationFilterWidget.ui \
    widgets/ddmDeltaPopulationFilterWidget.ui

Debug:DESTDIR   = debug
Release:DESTDIR = release

MOC_DIR     = $${DESTDIR}
OBJECTS_DIR = $${DESTDIR}
UI_DIR      = $${DESTDIR}

#INCLUDEPATH += bld $${DESTDIR} $${DESTDIR}/..


RESOURCES += \
    icons.qrc

