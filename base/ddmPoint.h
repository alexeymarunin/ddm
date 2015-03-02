#ifndef DDM_POINT_H
#define DDM_POINT_H

#include "base/ddmObject.h"
#include <QPointF>

/**
 * Класс ddmPoint описывает точку на карте
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
class ddmPoint : public ddmObject
{
    Q_OBJECT

public:

    ddmPoint( double x = 0.0, double y = 0.0, QObject* parent = 0 );

    Q_PROPERTY( double x READ x )
    Q_PROPERTY( double y READ y )

    double x() const;
    double y() const;

    virtual ~ddmPoint();

protected:

    QString m_id;
    QPointF m_point;

};

typedef ddmPoint* ddmPointPtr;

#endif // DDM_POINT_H
