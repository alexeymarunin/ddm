#ifndef DDMMAP_H
#define DDMMAP_H

#include <QWebView>

class ddmContainer;

class ddmMap: public QWebView
{
public:
    ddmMap( QWidget* parent );

    void setCenter( const QPointF& point );

public slots:
    void replyFinished(QNetworkReply*);
    void loadCoordinates();
    void geoCode(const QString &address);
    void resize( int w, int h );

    void drawPolygon( ddmContainer* boundary );

signals:
    void reloadMap();

private:
    QNetworkAccessManager *m_manager;
    QList<QPointF> m_coordinates;
    int m_pendingRequests;
};


#endif // DDMMAP_H
