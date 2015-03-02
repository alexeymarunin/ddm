#include "base/ddmBoundary.h"

/**
 * @brief   Конструктор по умолчанию
 *
 * @param   id Идентификатор контура
 * @param   x Координата X контура
 * @param   y Координата Y контура
 * @param   square Площадь контура
 * @param   parent Родитель контура (обычно ddmCounty)
 * @author  Марунин А.В.
 * @since   2.1
 */
ddmBoundary::ddmBoundary( int id, double x, double y, double square, QObject* parent ) : ddmMapObject( id, parent )
{
    this->create( id, x, y, square );
}

ddmBoundary::ddmBoundary( const QSqlRecord& record, QObject* parent ) : ddmMapObject( 0, parent )
{
    this->create( record );
}

/**
 * Создает контур из записи SQL-запроса
 *
 * @param   record Запись SQL-запроса
 * @author  Марунин А.В.
 * @since   2.1
 */
void ddmBoundary::create( const QSqlRecord& record )
{
    int id = record.value( "boundary_id" ).toInt();
    double x = record.value( "center_x" ).toDouble();
    double y = record.value( "center_y" ).toDouble();
    double square = record.value( "boundary_square" ).toDouble();

    this->create( id, x, y, square );
}

/**
 * Создает контур по заданным параметрам
 *
 * @param   id Уникальный идентификатор
 * @param   x Координата X центра контура
 * @param   y Координата Y центра контура
 * @param   square Площадь контура
 * @author  Марунин А.В.
 * @since   2.1
 */
void ddmBoundary::create( int id, double x, double y, double square )
{
    this->m_id = id;
    this->m_square = square;
    this->m_center = new ddmCenterPoint( x, y, this );

    this->setObjectName( QString( "boundary_%1" ).arg( this->m_id ) );
}

/**
 * Добавляет в контур вершину с указанными координатами
 * Создает объект типа ddmPoint, владельцем которого становится контур.
 *
 * @param   x Координата X вершины
 * @param   y Координата Y вершины
 * @return  Указатель на вершину
 * @author  Марунин А.В.
 * @since   2.1
 */
ddmPointPtr ddmBoundary::addVertex( double x, double y )
{
    ddmPointPtr vertex = new ddmPoint( x, y, this );
    return vertex;
}

/**
 * Добавляет в контур вершину из SQL-запроса
 * Создает объект типа ddmPoint, владельцем которого становится контур.
 *
 * @param   record Запись SQL-запроса
 * @return  Указатель на вершину
 * @author  Марунин А.В.
 * @since   2.1
 */
ddmPointPtr ddmBoundary::addVertex( const QSqlRecord& record )
{
    double x = record.value( "x" ).toDouble();
    double y = record.value( "y" ).toDouble();
    return this->addVertex( x, y );
}

/**
 * Возвращает вершину с заданным идентификатором
 *
 * @param   id Идентификатор вершины
 * @return  Найденная вершина или NULL
 * @author  Марунин А.В.
 * @since   2.1
 */
ddmPointPtr ddmBoundary::vertex( int id ) const
{
    return qobject_cast<ddmPoint*>( this->findChildById( id, "ddmPoint" ) );
}

/**
 * Возвращает количество вершин контура
 *
 * @return  Количество вершин контура
 * @author  Марунин А.В.
 * @since   2.1
 */
int ddmBoundary::vertexCount() const
{
    return this->childCount( "ddmPoint" );
}

/**
 * Возвращает список вершин контура
 *
 * @return  Список QVariantList, содержащий вершины контура
 * @author  Марунин А.В.
 * @since   2.1
 */
QVariantList ddmBoundary::vertices() const
{
    return this->findChilds( "ddmPoint" );
}

/**
 * Возвращает площадь контура
 *
 * @return  Площадь контура
 * @author  Марунин А.В.
 * @since   2.1
 */
double ddmBoundary::square() const
{
    return this->m_square;
}

/**
 * Возвращает центр контура
 * Используется при передачи данных в JavaScript
 *
 * @return  Объект типа QVariantMap с двумя элементами - x и y
 * @author  Марунин А.В.
 * @since   2.1
 */
QVariantMap ddmBoundary::center() const
{
    QVariantMap center;
    center["x"] = this->m_center->x();
    center["y"] = this->m_center->y();
    return center;
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.1
 */
ddmBoundary::~ddmBoundary()
{
}

