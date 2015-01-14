#ifndef DDM_MAP_VIEW_H
#define DDM_MAP_VIEW_H

#include <QWebView>

class ddmMapView : public QWebView
{
public:
    ddmMapView( QWidget* parent );

    void selectCounty( int id );

    void injectModel( QObject* model );

    // увеличить приближение
    void increaseZoomLevel();

    // уменьшить приближение
    void decreaseZoomLevel();

public slots:
    void replyFinished( QNetworkReply* reply );
    void loadCoordinates();
    void geoCode(const QString &address);
    void resize( int w, int h );

signals:
    void reloadMap();

private:
    QNetworkAccessManager*  m_manager;

    QList<QPointF> m_coordinates;
    int m_pendingRequests;
};


#endif // DDM_MAP_VIEW_H
