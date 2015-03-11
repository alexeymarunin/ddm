#include "base/ddmCenterPoint.h"

/**
 * @brief   Конструктор по умолчанию
 *
 * @param   x Координата X точки
 * @param   y Координата Y точки
 * @param   parent Родитель точки
 * @author  Марунин А.В.
 * @since   2.1
 */
ddmCenterPoint::ddmCenterPoint( double x, double y, QObject* parent ) : ddmPoint( x, y, parent )
{
    this->setObjectName( "center" );
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.1
 */
ddmCenterPoint::~ddmCenterPoint()
{
}

