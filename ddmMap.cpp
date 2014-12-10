#include "ddmMap.h"

#include <QNetworkAccessManager>
#include <QUrl>


ddmMap::ddmMap( QWidget* parent ) : QWebView(parent), m_pendingRequests(0)
{
    m_manager = new QNetworkAccessManager(this);

    connect( m_manager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( replyFinished(QNetworkReply*) ));

    connect(this,SIGNAL( reloadMap() ), this,SLOT( loadCoordinates() ));
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


void ddmMap::resizeEvent( QResizeEvent* event )
{

}
