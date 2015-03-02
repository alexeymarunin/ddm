#include "base/ddmPoint.h"

/**
 * Конструктор класса
 *
 * @param   X-координата точки
 * @param   Y-координата точки
 * @param   parent Родитель (владелец) точки (обычно ddmBoundary)
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmPoint::ddmPoint( double x, double y, QObject* parent ) : ddmObject( parent )
{
    this->m_point = QPointF( x, y );
}

/**
 * Возвращает X-координату точки
 *
 * @return  Вещественное число
 * @author  Марунин А.В.
 * @since   2.0
 */
double ddmPoint::x() const
{
    return this->m_point.x();
}

/**
 * Возвращает Y-координату точки
 *
 * @return  Вещественное число
 * @author  Марунин А.В.
 * @since   2.0
 */
double ddmPoint::y() const
{
    return this->m_point.y();
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmPoint::~ddmPoint()
{
}

