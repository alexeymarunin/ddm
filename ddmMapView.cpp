#include <QNetworkAccessManager>
#include <QUrl>
#include <QWebFrame>
#include <QWebElement>

#include "ddmMapView.h"
#include "ddmContainer.h"


ddmMapView::ddmMapView( QWidget* parent ) : QWebView(parent), m_pendingRequests(0)
{
    m_manager = new QNetworkAccessManager(this);

    connect( m_manager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( replyFinished(QNetworkReply*) ));

    connect(this,SIGNAL( reloadMap() ), this,SLOT( loadCoordinates() ));
}


void ddmMapView::setCenter( const QPointF &point )
{
    QString scriptStr;
    scriptStr = QObject::tr( "setMapCenter( %1, %2 );" ).arg( point.x() ).arg( point.y() );
    page()->currentFrame()->documentElement().evaluateJavaScript( scriptStr );
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

    QString scriptStr = QObject::tr( "setWebPageSize( %1, %2 );" ).arg( w ).arg( h );
    page()->mainFrame()->evaluateJavaScript( scriptStr );
}


void ddmMapView::drawPolygon( ddmContainer* boundary )
{
    if( boundary == NULL )
        return;

    page()->currentFrame()->addToJavaScriptWindowObject( "container", boundary );
    page()->currentFrame()->documentElement().evaluateJavaScript( "drawPolygon( container );" );
    page()->currentFrame()->documentElement().evaluateJavaScript( "delete container;" );
}
