#ifndef DDM_BOUNDARY_H
#define DDM_BOUNDARY_H

#include <QPointF>
#include <QMap>
#include <QVariantMap>

#include "ddmObject.h"

class ddmCounty;

typedef QList<QPointF> ddmPointList;

class ddmBoundary : public ddmObject
{
    Q_OBJECT
public:
    explicit ddmBoundary( int id = 0, ddmCounty* county = 0 );

    void addVertex( const QPointF& point );
    QPointF vertex( int id ) const;
    ddmPointList& vertices() const;

    void setCounty( ddmCounty* county );
    ddmCounty* county() const;

    void setSquare( double square );
    double square() const;

    void setCenter( const QPointF& point );
    QPointF center() const;

    QVariantMap getProperties() const;

    Q_PROPERTY( QVariantMap props READ getProperties );

    virtual ~ddmBoundary();

signals:

public slots:

private:
    ddmPointList m_vertices;
    double      m_square;
    QPointF     m_center;

};

#endif // DDM_BOUNDARY_H
