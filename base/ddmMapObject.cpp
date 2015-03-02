#include "base/ddmMapObject.h"

/**
 * Конструктор класса
 *
 * @param   id Целое число, определяющее уникальный идентификатор объекта
 * @param   parent Родитель (владелец) объекта
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmMapObject::ddmMapObject( int id, QObject* parent ) : ddmObject( parent ),
    m_id( id ), m_visible( false )
{
}

/**
 * Возвращает уникальный идентификатор объекта
 *
 * @return  Целое число
 * @author  Марунин А.В.
 * @since   2.0
 */
int ddmMapObject::id() const
{
    return this->m_id;
}

/**
 * Возвращает видимость объекта на карте
 *
 * @return  true, если объект отображается
 * @author  Марунин А.В.
 * @since   2.0
 */
bool ddmMapObject::visible() const
{
    return this->m_visible;
}

/**
 * Изменяет видимость объекта на карте.
 *
 * Если состояние действительно изменяется, то испускается сигнал visibleChanged()
 *
 * @param   visible Флаг, определяющий, будет ли виден объект на карте
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmMapObject::setVisible( bool visible )
{
    if ( this->m_visible != visible )
    {
        this->m_visible = visible;
        Q_EMIT visibleChanged();
    }
}

/**
 * Показывает объект на карте
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmMapObject::show()
{
    this->setVisible( true );
}

/**
 * Скрывает объект с карты
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmMapObject::hide()
{
    this->setVisible( false );
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmMapObject::~ddmMapObject()
{
}

