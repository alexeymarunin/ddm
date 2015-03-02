#include <QDebug>
#include "base/ddmState.h"
#include "base/ddmCounty.h"

/**
 * Конструктор класса
 *
 * @param   id Уникальный идентификатор штата
 * @param   geographicName Географическое название штата
 * @param   parent Родитель штата
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmState::ddmState( int id, const QString& geographicName, QObject* parent ) : ddmMapObject( id, parent )
{
    this->create( id, geographicName );
}

/**
 * Конструктор
 *
 * @param   record SQL-запись, из которой будут взяты параметры штата
 * @param   parent Родитель штата
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmState::ddmState( const QSqlRecord& record, QObject* parent ) : ddmMapObject( 0, parent )
{
    this->create( record );
}

/**
 * Инициализирует штат из записи SQL-запроса
 *
 * @param   record Запись SQL-запроса
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmState::create( const QSqlRecord& record )
{
    int id = record.value( "state_id" ).toInt();
    QString geographicName = record.value( "state_name" ).toString();

    this->create( id, geographicName );
}

/**
 * Инициализирует штат заданными параметрами
 *
 * @param   id Уникальный идентификатор штата
 * @param   geographicName Географическое название штата
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmState::create( int id, const QString& geographicName )
{
    this->m_id = id;
    this->m_geographicName = geographicName;

    this->setObjectName( QString( "state_%1" ).arg( this->m_id ) );
}

/**
 * Добавляет графство в штат
 *
 * Фактически задает штат в качестве владельца графства.
 *
 * @param   county Указатель на добавляемое графство
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmState::addCounty( ddmCounty* county )
{
    county->setParent( this );
}

/**
 * Возвращает географическое имя штата
 *
 * @return  Географическое имя штата
 * @author  Марунин А.В.
 * @since   2.0
 */
QString ddmState::geographicName() const
{
    return this->m_geographicName;
}

/**
 * Возвращает список графств штата
 *
 * @return  Объект типа QVariantMap, каждый элемент которого есть указатель на
 *          объект типа ddmCounty, "завернутый" в QVariant
 * @author  Марунин А.В.
 * @since   2.0
 */
QVariantList ddmState::counties() const
{
    return this->findChilds( "ddmCounty" );
}

/**
 * Возвращает количество графств штата
 *
 * @return  Целое число
 * @author  Марунин А.В.
 * @since   2.0
 */
int ddmState::countyCount() const
{
    return this->childCount( "ddmCounty" );
}

/**
 * Ищет графство с заданным id
 *
 * @param   id Целое число - идентификатор графства
 * @return  Указатель на найденное графство или NULL
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmCounty* ddmState::county( int id ) const
{
    return qobject_cast<ddmCounty*>( this->findChildById( id, "ddmCounty" ) );
}

/**
 * Ищет графство с заданным именем
 *
 * @param   geographicName Имя графства
 * @return  Указатель на найденное графство или NULL
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmCounty* ddmState::county( const QString& geographicName ) const
{
    ddmCounty* county = NULL;
    QVariantList counties = this->counties();
    foreach ( QVariant obj, counties )
    {
        ddmCounty* value = obj.value<ddmCounty*>();
        if ( value->geographicName() == geographicName )
        {
            county = value;
            break;
        }
    }
    return county;
}

/**
 * Возвращает список имен графств в штате
 *
 * @return  Объект типа QStringList
 * @author  Марунин А.В.
 * @since   2.0
 */
QStringList ddmState::countyNames() const
{
    QStringList items;
    QVariantList counties = this->counties();
    foreach ( QVariant obj, counties )
    {
        ddmCounty* value = obj.value<ddmCounty*>();
        items.append( value->geographicName() );
    }
    return items;
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmState::~ddmState()
{
}
