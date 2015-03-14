#ifndef DDM_MAP_VIEW_H
#define DDM_MAP_VIEW_H

#include <QWebView>
#include <QVBoxLayout>
#include <QVariantMap>

class ddmFilter;

/**
 * Класс ddmMapView определяет мини-браузер, в котором отображается
 * веб-страница с Google Map
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
class ddmMapView : public QWebView
{
    Q_OBJECT

public:

    ddmMapView( ddmFilter* filter, QWidget* parent = NULL );

    void addToJavaScriptWindowObject( const QString& name, QObject* object );
    QVariant evaluateJavaScript( const QString& scriptSource );

    // увеличить приближение
    void increaseZoomLevel();

    // уменьшить приближение
    void decreaseZoomLevel();

    void setCenter( double x, double y );
    void setCenter( const QVariantMap& center );

    void setMarker( double x, double y );
    void setMarker( const QVariantMap& point );

    void fitCounty( int county_id );

    bool mapReady() const;

    ddmFilter* filter() const;

    virtual ~ddmMapView();

Q_SIGNALS:

    void mousemove( double lat, double lng );
    void resized( int width, int height );
    void loaded();

    void javaScriptWindowObjectCleared();
    void reloadMap();

public Q_SLOTS:

    void slotReplyFinished( QNetworkReply* reply );
    void slotJavaScriptWindowObjectCleared();
    void resize( int w, int h );
    void show();
    void hide();

protected Q_SLOTS:

    void slotLoaded();

protected:

    QNetworkAccessManager*  m_manager;
    ddmFilter*              m_filter;
    int                     m_pendingRequests;
    bool                    m_mapReady;
    QVBoxLayout*            m_mapLayout;

};


#endif // DDM_MAP_VIEW_H
