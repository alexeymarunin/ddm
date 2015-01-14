#include <QNetworkAccessManager>
#include <QUrl>
#include <QWebFrame>
#include <QWebElement>

#include "ddmMapView.h"


ddmMapView::ddmMapView( QWidget* parent ) : QWebView( parent ),
    m_pendingRequests( 0 )
{
    m_manager = new QNetworkAccessManager(this);

    connect( m_manager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( replyFinished(QNetworkReply*) ));

    QWebFrame* frame = this->page()->mainFrame();
    // отключаем полосы прокрутки
    frame->setScrollBarPolicy( Qt::Vertical,   Qt::ScrollBarAlwaysOff );
    frame->setScrollBarPolicy( Qt::Horizontal, Qt::ScrollBarAlwaysOff );
}

void ddmMapView::selectCounty( int id )
{
    QString script;
    script = QObject::tr( "ddm_select_county( %1 );" ).arg( id );
    this->page()->currentFrame()->documentElement().evaluateJavaScript( script );
}

void ddmMapView::injectModel( QObject* model )
{
    QWebFrame* frame = this->page()->currentFrame();
    QString script;
    frame->addToJavaScriptWindowObject( "ddm_model", model );
}


void ddmMapView::increaseZoomLevel()
{
    if( !page() )
        return;

    QString scriptStr = QObject::tr( "ddm_increase_map_zoom();" );
    page()->mainFrame()->evaluateJavaScript( scriptStr );
}


void ddmMapView::decreaseZoomLevel()
{
    if( !page() )
        return;

    QString scriptStr = QObject::tr( "ddm_decrease_map_zoom();" );
    page()->mainFrame()->evaluateJavaScript( scriptStr );
}


void ddmMapView::replyFinished( QNetworkReply* reply )
{

}


void ddmMapView::loadCoordinates()
{

}


void ddmMapView::geoCode(const QString& address )
{

}


void ddmMapView::resize( int w, int h )
{
    if( !page() )
        return;

    QString scriptStr = QObject::tr( "ddm_map_resize( %1, %2 );" ).arg( w ).arg( h );
    page()->mainFrame()->evaluateJavaScript( scriptStr );
}

