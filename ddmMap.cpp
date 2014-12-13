#include "ddmMap.h"

#include <QNetworkAccessManager>
#include <QUrl>
#include <QWebFrame>
#include <QWebElement>


ddmMap::ddmMap( QWidget* parent ) : QWebView(parent), m_pendingRequests(0)
{
    m_manager = new QNetworkAccessManager(this);

    connect( m_manager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( replyFinished(QNetworkReply*) ));

    connect(this,SIGNAL( reloadMap() ), this,SLOT( loadCoordinates() ));
}


void ddmMap::updateMap( const QPointF &point )
{
    QString scriptStr;
    scriptStr = QObject::tr( "setMapCenter( %1, %2 );" ).arg( point.x() ).arg( point.y() );
    page()->currentFrame()->documentElement().evaluateJavaScript( scriptStr );
}


void ddmMap::replyFinished( QNetworkReply* reply )
{

}


void ddmMap::loadCoordinates()
{

}


void ddmMap::geoCode(const QString& address )
{

}


void ddmMap::resize( int w, int h )
{
    if( !page() )
        return;

    QString scriptStr = QObject::tr( "setWebPageSize( %1, %2 );" ).arg( w ).arg( h );
    page()->mainFrame()->evaluateJavaScript( scriptStr );
}
