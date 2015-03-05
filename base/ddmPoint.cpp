#include <QtGlobal>
#include "base/ddmPoint.h"

/**
 * Точность, используемая при сравнении точек на совпадение
 * @see equal()
 */
#define DDM_POINT_PRECISION 1.0e-05

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
 * Определяет, совпадает ли точка с точкой по указанным координатам
 *
 * @param   x X-координата точки
 * @param   y Y-координата точки
 * @return  true, если координаты совпадают с точностью DDM_POINT_PRECISION
 * @author  Марунин А.В.
 * @since   2.3
 */
bool ddmPoint::equal( double x, double y ) const
{
    double dx = this->x() - x;
    double dy = this->y() - y;
    return ( qAbs( dx ) <= DDM_POINT_PRECISION && qAbs( dy ) <= DDM_POINT_PRECISION );
}

/**
 * Определяет, совпадает ли точка с заданной точкой
 *
 * @param   point Точка, с которой производится сравнение
 * @return  true, если координаты совпадают с точностью DDM_POINT_PRECISION
 * @author  Марунин А.В.
 * @since   2.3
 */
bool ddmPoint::equal( ddmPoint* point ) const
{
    return ( point ? this->equal( point->x(), point->y() ) : false );
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

