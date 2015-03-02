#include <QDebug>
#include "ddmVersion.h"
#include "ddmMainWindow.h"
#include "widgets/ddmCentralWidget.h"
#include "ddmApplication.h"

ddmApplication::ddmApplication( int& argc, char** argv ) : QApplication( argc, argv )
{
    qDebug() << "App version:" << this->version();
#ifdef QT_DEBUG
    qDebug() << "Mode: Debug";
#else
    qDebug() << "Mode: Release";
#endif
    this->m_mainWindow = new ddmMainWindow;

    ddmCentralWidget* centralWidget = new ddmCentralWidget( this->mainWindow() );
    this->mainWindow()->setCentralWidget( centralWidget );

    QString title = QString( "Distance Decay Map, v.%1" ).arg( this->version() );
#ifdef QT_DEBUG
    title += QString( " [DEBUG MODE]" );
#endif
    this->m_mainWindow->setWindowTitle( title );
    this->m_mainWindow->show();

    QComboBox* comboFilter = centralWidget->comboFilter();
    QObject::connect( comboFilter, SIGNAL( currentIndexChanged(int) ), centralWidget, SLOT( slotChangedFilter(int) ) );

    centralWidget->setCurrentFilter( 0 );
}

ddmMainWindow* ddmApplication::mainWindow() const
{
    return this->m_mainWindow;
}

ddmCentralWidget* ddmApplication::centralWidget() const
{
    return qobject_cast<ddmCentralWidget*>( this->mainWindow()->centralWidget() );
}

QString ddmApplication::version() const
{
    return QString( "%1.%2.%3" ).arg( DDM_MAJOR_VERSION ).arg( DDM_MINOR_VERSION ).arg( DDM_BUILD_NUMBER );
}

