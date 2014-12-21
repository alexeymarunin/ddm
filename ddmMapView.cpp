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

}

void ddmMapView::setCenter( const QPointF &point )
{
    QString script;
    script = QObject::tr( "ddm_set_map_center( %1, %2 );" ).arg( point.x() ).arg( point.y() );
    this->page()->currentFrame()->documentElement().evaluateJavaScript( script );
}

void ddmMapView::injectModel( QObject* model )
{
    QWebFrame* frame = this->page()->currentFrame();
    QString script;
    frame->addToJavaScriptWindowObject( "ddm_model", model );
    //script = QObject::tr( "ddm_add_counties( ddm_model.countiesJSON ); alert( ddm_model )" );
    //frame->documentElement().evaluateJavaScript( script );
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

/*
void ddmMapView::drawPolygon( ddmContainer* boundary )
{
    if( boundary == NULL )
        return;

    page()->currentFrame()->addToJavaScriptWindowObject( "container", boundary );
    page()->currentFrame()->documentElement().evaluateJavaScript( "drawPolygon( container );" );
    page()->currentFrame()->documentElement().evaluateJavaScript( "delete container;" );
}
*/
