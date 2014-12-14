#ifndef DDMMAP_H
#define DDMMAP_H

#include <QWebView>

class QPointF;

class ddmMap: public QWebView
{
public:
    ddmMap( QWidget* parent );

    void updateMap( const QPointF& point );

public slots:
    void replyFinished(QNetworkReply*);
    void loadCoordinates();
    void geoCode(const QString &address);
    void resize( int w, int h );

signals:
    void reloadMap();

private:
    QNetworkAccessManager *m_manager;
    QList<QPointF> m_coordinates;
    int m_pendingRequests;
};


#endif // DDMMAP_H
