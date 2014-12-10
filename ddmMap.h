#ifndef DDMMAP_H
#define DDMMAP_H

#include <QWebView>

class ddmMap: public QWebView
{
public:
    ddmMap( QWidget* parent );

public slots:
    void replyFinished(QNetworkReply*);
    void loadCoordinates();
    void geoCode(const QString &address);

protected:
    void resizeEvent(QResizeEvent *event);

signals:
    void reloadMap();

private:
    QNetworkAccessManager *m_manager;
    QList<QPointF> m_coordinates;
    int m_pendingRequests;
};


#endif // DDMMAP_H
