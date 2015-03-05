#ifndef DDM_BOUNDARY_H
#define DDM_BOUNDARY_H

#include <QPointF>
#include <QVariantMap>
#include <QVariantList>
#include <QSqlRecord>
#include <QVariant>

#include "base/ddmMapObject.h"
#include "base/ddmCenterPoint.h"


/**
 * Класс ddmBoundary описывает контур графства
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
class ddmBoundary : public ddmMapObject
{
    Q_OBJECT

public:

    ddmBoundary( int id = 0, double x = 0.0, double y = 0.0, double square = 0.0, QObject* parent = 0 );
    ddmBoundary( const QSqlRecord& record, QObject* parent = 0 );

    Q_PROPERTY( double square READ square )
    Q_PROPERTY( QVariantMap center READ center )
    Q_PROPERTY( QVariantList vertices READ vertices )
    Q_PROPERTY( int vertexCount READ vertexCount )

    ddmPointPtr addVertex( double x, double y );
    ddmPointPtr addVertex( const QSqlRecord& record );

    int vertexCount() const;
    ddmPointPtr vertex( int id ) const;
    QVariantList vertices() const;

    double square() const;
    QVariantMap center() const;

    virtual ~ddmBoundary();

protected:

    ddmCenterPoint* m_center;
    double          m_square;

    virtual void create( const QSqlRecord& record );
    void create( int id, double x, double y, double square );

};

#endif // DDM_BOUNDARY_H
