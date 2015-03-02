#include <QtGlobal>
#include <QtDebug>
#include <QDir>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QWebFrame>
#include <QWebElement>

#include "filters/ddmFilter.h"
#include "ddmMapViewPage.h"
#include "ddmMainWindow.h"
#include "ddmApplication.h"
#include "ddmMapView.h"

#define DDM_INDEX_PAGE "index.html"

/**
 * @brief ddmMapView::ddmMapView
 * @param parent
 */
ddmMapView::ddmMapView( ddmFilter* filter, QWidget* parent ) : QWebView( parent ),
    m_filter( filter ), m_pendingRequests( 0 )
{
    this->setContentsMargins( 0, 0, 0, 0 );
    this->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
    this->setMinimumSize( 500, 300 );

    this->m_manager = new QNetworkAccessManager( this );

    ddmMapViewPage* mapPage = new ddmMapViewPage( this );
    this->setPage( mapPage );

    QWebFrame* pageFrame = mapPage->mainFrame();

    // отключаем полосы прокрутки
    pageFrame->setScrollBarPolicy( Qt::Vertical,   Qt::ScrollBarAlwaysOff );
    pageFrame->setScrollBarPolicy( Qt::Horizontal, Qt::ScrollBarAlwaysOff );

    QString currentDir = QDir::current().absolutePath();
    QStringList dirs;
    dirs.append( currentDir );
    dirs.append( QObject::tr( "%1/../.." ).arg( currentDir ) );
    dirs.append( QObject::tr( "%1/.." ).arg( currentDir ) );

    QString pathIndex;
    foreach ( QString dir, dirs )
    {
        QString path = QDir( QObject::tr( "%1/%2" ).arg( dir ).arg( DDM_INDEX_PAGE ) ).absolutePath();
        if ( QFile::exists( path ) )
        {
            qDebug() << "Found page " << path;
            pathIndex = path;
            break;
        }
    }

    Q_ASSERT( !pathIndex.isEmpty() );

    QUrl url = QUrl::fromLocalFile( pathIndex );
    this->setUrl( url );

    QObject::connect( this, SIGNAL( loaded() ), this, SLOT( slotLoaded() ) );
    QObject::connect( pageFrame, SIGNAL( javaScriptWindowObjectCleared() ), this, SLOT( slotJavaScriptWindowObjectCleared() ) );
    QObject::connect( this->m_manager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( slotReplyFinished(QNetworkReply*) ) );

    ddmMainWindow* mainWindow = ddmApp->mainWindow();
    connect( this, SIGNAL( mousemove(double,double) ), mainWindow, SLOT( slotMapMouseMove(double,double) ) );

}

bool ddmMapView::mapReady() const
{
    return this->m_mapReady;
}

/**
 * @brief ddmMapView::slotJavaScriptWindowObjectCleared
 */
void ddmMapView::slotJavaScriptWindowObjectCleared()
{
    this->m_mapReady = false;
    this->addToJavaScriptWindowObject( "ddmMapView", this );
    Q_EMIT javaScriptWindowObjectCleared();
}

void ddmMapView::slotLoaded()
{
    this->m_mapReady = true;
}

void ddmMapView::increaseZoomLevel()
{
    this->evaluateJavaScript( QObject::tr( "ddm_increase_map_zoom();" ) );
}


void ddmMapView::decreaseZoomLevel()
{
    this->evaluateJavaScript( QObject::tr( "ddm_decrease_map_zoom();" ) );
}

void ddmMapView::setCenter( const QVariantMap& center )
{
    double x = center["x"].toDouble();
    double y = center["y"].toDouble();
    this->setCenter( x, y );
}

void ddmMapView::setCenter( double x, double y )
{
    if ( this->mapReady() )
    {
        this->evaluateJavaScript( QString( "window.ddmMap.panTo( %1, %2 );" ).arg( x ).arg( y ) );
    }
}

void ddmMapView::setMarker( const QVariantMap& center )
{
    double x = center["x"].toDouble();
    double y = center["y"].toDouble();
    this->setMarker( x, y );
}

void ddmMapView::setMarker( double x, double y )
{
    if ( this->mapReady() )
    {
        this->evaluateJavaScript( QString( "window.ddmMap.setMarker( %1, %2 );" ).arg( x ).arg( y ) );
    }
}


void ddmMapView::slotReplyFinished( QNetworkReply* reply )
{
    Q_UNUSED( reply );
}


/**
 * @brief ddmMapView::addToJavaScriptWindowObject
 * @param name
 * @param object
 */
void ddmMapView::addToJavaScriptWindowObject( const QString& name, QObject* object )
{
    if ( this->page() )
    {
        this->page()->mainFrame()->addToJavaScriptWindowObject( name, object );
    }
}

/**
 * @brief ddmMapView::evaluateJavaScript
 * @param scriptSource
 * @return
 */
QVariant ddmMapView::evaluateJavaScript( const QString& scriptSource )
{
    QVariant result;
    if ( this->page() )
    {
        result = this->page()->mainFrame()->evaluateJavaScript( scriptSource );
    }

    return result;
}

void ddmMapView::resize( int width, int height )
{
    qDebug() << "ddmMapView::resize";
    Q_EMIT resized( width, height );
}

ddmFilter* ddmMapView::filter() const
{
    return this->m_filter;
}
