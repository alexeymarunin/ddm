#include <QVariant>
#include "base/ddmObject.h"

/**
 * Конструктор класса
 * @param   parent Родитель (владелец) объекта
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmObject::ddmObject( QObject* parent ) : QObject( parent )
{
}

/**
 * Находит прямого потомка с заданным именем класса и id
 * @param   id Идентификатор потомка (не индекс!)
 * @param   className Имя класса потомка
 * @return  Указатель на объект типа QObject
 */
QObject* ddmObject::findChildById( int id, const QString& className ) const
{
    QObject* child = NULL;
    QObjectList children = this->children();
    foreach( QObject* obj, children )
    {
        QVariant property = obj->property( "id" );
        int child_id = property.isValid() ? property.toInt() : 0;
        if ( child_id == 0 ) continue;
        QString child_class = obj->metaObject()->className();
        if ( child_id == id && ( className.isEmpty() || child_class == className ) )
        {
            child = obj;
            break;
        }
    }
    return child;
}

/**
 * Находит всех прямых потомков, имя класса которых совпадает с заданным
 *
 * @param   className Имя класса потомков
 * @return  Объект типа QVariantList
 * @author  Марунин А.В.
 * @since   2.0
 */
QVariantList ddmObject::findChilds( const QString& className ) const
{
    QVariantList childs;
    QObjectList children = this->children();
    foreach( QObject* child, children )
    {
        if ( child->metaObject()->className() != className ) continue;
        childs.append( QVariant::fromValue( child ) );
    }
    return childs;
}

/**
 * Подсчитывает количество прямых потомков заданного класса
 *
 * @param   className Имя класса потомков
 * @return  Целое число
 * @author  Марунин А.В.
 * @since   2.0
 */
int ddmObject::childCount( const QString& className ) const
{
    return this->findChilds( className ).size();
}

/**
 * Возвращает имя класса
 *
 * @return  Строка с именем класса
 * @author  Марунин А.В.
 * @since   2.0
 */
QString ddmObject::className() const
{
    return this->metaObject()->className();
}

/**
 * Определяет, совпадает ли имя класса с заданным
 *
 * @param   className Имя требуемого класса
 * @return  true, если имя класса сопадает с заданным
 * @author  Марунин А.В.
 * @since   2.0
 */
bool ddmObject::isClass( const QString& className ) const
{
    return ( this->className() == className );
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmObject::~ddmObject()
{
}

