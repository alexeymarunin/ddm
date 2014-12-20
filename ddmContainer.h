#ifndef DDMCONTAINER
#define DDMCONTAINER

#include <QObject>
#include <QVariant>


class ddmContainer : public QObject
{
  Q_OBJECT

public:

    ~ddmContainer();

    void appendPoint( QVariant lat, QVariant lon );

    void setName( QVariant name );

    void setId( QVariant id );

    void setCenter( QVariant center );

    void clear();


public slots:

    QVariantList points();

    QVariant id();

    QVariant name();

    QVariant center();

    int length();

private:
    QVariantList m_points;

    QVariant m_name;

    QVariant m_id;

    QVariant m_center;
};


#endif // DDMCONTAINER

