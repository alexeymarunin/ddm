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
    m_id( id )
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
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmMapObject::~ddmMapObject()
{
}

