#ifndef DDM_CENTER_POINT_H
#define DDM_CENTER_POINT_H

#include "base/ddmPoint.h"

/**
 * Класс ddmCenterPoint описывает центральную точку
 * В отличие от ddmPoint, при создании автоматически задается objectName как "center",
 * чтобы впоследствии к ним можно было обращаться в JavaScript
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
class ddmCenterPoint : public ddmPoint
{
    Q_OBJECT

public:

    ddmCenterPoint( double x = 0.0, double y = 0.0, QObject* parent = 0 );
    virtual ~ddmCenterPoint();
};

#endif // DDM_CENTER_POINT_H
