#ifndef DDMCONTAINER
#define DDMCONTAINER

#include <QObject>
#include <QDebug>



class ddmContainer : public QObject
{
  Q_OBJECT

public:

    ~ddmContainer();

    void appendLat( QVariant lat );

    void appendLon( QVariant lon );

    void clear();


public slots:

    QVariantList getLatList();

    QVariantList getLonList();

    int length();

private:
    QVariantList m_lat;
    QVariantList m_lon;
};


#endif // DDMCONTAINER

