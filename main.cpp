#include <QApplication>
#include "mainwindow.h"
#include "ddmVersion.h"

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );
    MainWindow w;

    QString version = QString( "%1.%2.%3" ).arg( DDM_MAJOR_VERSION ).arg( DDM_MINOR_VERSION ).arg( DDM_BUILD_NUMBER );
    QString title = QString( "Distance Decay Map, v.%1" ).arg( version );
    w.setWindowTitle( title );
    w.show();

    return app.exec();
}
