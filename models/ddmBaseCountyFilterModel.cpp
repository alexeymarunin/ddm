#include <QSqlRecord>

#include "models/ddmBaseCountyFilterModel.h"

/**
 * Конструктор класса
 *
 * @param   parent Владелец модели
 * @author  Марунин А.В.
 * @since   2.8
 */
ddmBaseCountyFilterModel::ddmBaseCountyFilterModel( QObject* parent ) : ddmFilterModel( parent ),
    m_currentState( NULL ), m_currentCounty( NULL )
{
    this->updateStateNames();
    ddmState* state = this->loadState( 1 );
    ddmCounty* county = state->county( 1 );
    this->setCurrentCounty( county );
}

/**
 * Загружает данные из БД
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmBaseCountyFilterModel::reloadData()
{
    if ( this->currentState() )
    {
        this->loadState( this->currentState()->id() );
        this->updateCountyNames();
        if ( this->currentCounty() )
        {
            // Выбираем графства текущего штата
            QString sqlQuery = QString( "SELECT * FROM cache_boundaries WHERE county_id = %1" ).arg( this->currentCounty()->id() );
            this->execQuery( sqlQuery );
        }
    }
}

/**
 * Возвращает текущий штат
 *
 * @return  Указатель на объект типа ddmState или NULL
 * @author  Марунин А.В.
 * @since   2.8
 */
ddmState* ddmBaseCountyFilterModel::currentState() const
{
    return this->m_currentState;
}

/**
 * Задает текущим штат с заданным идентификатором
 *
 * Текущим графством становится первое по имени графство в штате.
 *
 * @param   id Идентификатор штата
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmBaseCountyFilterModel::setCurrentState( int state_id )
{
    Q_ASSERT( state_id > 0 );
    ddmState* state = this->findState( state_id );
    if ( !state )
    {
        state = this->loadState( state_id );
    }
    this->setCurrentState( state );
}

/**
 * Делает текущим штат с заданным названием
 *
 * Текущим графством становится первое по имени графство в штате.
 *
 * @param   geographicName Географическое название штат
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmBaseCountyFilterModel::setCurrentState( const QString& geographicName )
{
    ddmState* state = this->findState( geographicName );
    if ( !state )
    {
        int state_id = this->m_stateMap.key( geographicName, 0 );
        Q_ASSERT( state_id > 0 );
        this->setCurrentState( state_id );
    }
    else
    {
        this->setCurrentState( state );
    }
}

/**
 * Задает текущий штат
 *
 * @param   state Указатель на объект типа ddmState
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmBaseCountyFilterModel::setCurrentState( ddmState* state )
{
    if ( state )
    {
        if ( this->m_currentState != state )
        {
            this->m_currentState = state;
            if ( !this->currentCounty() || this->currentCounty()->state() != state )
            {
                QVariantList counties = state->counties();
                ddmCounty* county = counties.first().value<ddmCounty*>();
                this->m_currentCounty = county;
            }
            this->reloadData();
            Q_EMIT changed();
        }
    }
}

/**
 * Возвращает список имен штатов
 *
 * @return  Объект типа QStringList
 * @author  Марунин А.В.
 * @since   2.8
 */
QStringList ddmBaseCountyFilterModel::stateNames() const
{
    return QStringList( this->m_stateMap.values() );
}

/**
 * Возвращает текущее графство
 *
 * @return  Указатель на объект типа ddmCounty или NULL
 * @author  Марунин А.В.
 * @since   2.8
 */
ddmCounty* ddmBaseCountyFilterModel::currentCounty() const
{
    return this->m_currentCounty;
}

/**
 * Делает текущим графство с заданным идентификатором
 *
 * Если графства с таким id нет, то ничего не происходит.
 *
 * @param   id Идентификатор графства
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmBaseCountyFilterModel::setCurrentCounty( int id )
{
    bool found = false;

    QVariantList states = this->states();
    foreach ( QVariant obj, states )
    {
        ddmState* state = obj.value<ddmState*>();
        Q_ASSERT( state != NULL );
        ddmCounty* county = state->county( id );
        if ( county )
        {
            this->setCurrentCounty( county );
            found = true;
            break;
        }
    }

    if ( !found )
    {
        ddmDatabase& db = this->database();
        QString sql = QString( "SELECT state_id FROM ddm_counties WHERE id = %1 LIMIT 1" ).arg( id );
        QSqlQueryModel* query = db.select( sql );
        Q_ASSERT( db.hasErrors() == false );
        int state_id = query->data( query->index( 0, 0 ) ).toInt();
        if ( state_id )
        {
            ddmState* state = this->loadState( state_id );
            this->setCurrentState( state );
            this->setCurrentCounty( id );
        }
    }
}

/**
 * Делает текущим графство с заданномым названием
 *
 * Если графство не найдено, то ничего не происходит.
 *
 * @param   geographicName Название графства
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmBaseCountyFilterModel::setCurrentCounty( const QString& geographicName )
{
    bool found = false;

    QVariantList states = this->states();
    foreach ( QVariant obj, states )
    {
        ddmState* state = obj.value<ddmState*>();
        Q_ASSERT( state != NULL );
        ddmCounty* county = state->county( geographicName );
        if ( county )
        {
            this->setCurrentCounty( county );
            found = true;
            break;
        }
    }

    if ( !found )
    {
        ddmDatabase& db = this->database();
        QString sql = QString( "SELECT state_id FROM ddm_counties WHERE geographic_name = '%1'' LIMIT 1" ).arg( geographicName );
        QSqlQueryModel* query = db.select( sql );
        Q_ASSERT( db.hasErrors() == false );
        int state_id = query->data( query->index( 0, 0 ) ).toInt();
        if ( state_id )
        {
            ddmState* state = this->loadState( state_id );
            this->setCurrentState( state );
            this->setCurrentCounty( geographicName );
        }
    }
}

/**
 * Задает текущее графство
 *
 * @param   county Указатель на объект типа ddmCounty
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmBaseCountyFilterModel::setCurrentCounty( ddmCounty* county )
{
    if ( county )
    {
        if ( this->m_currentCounty != county )
        {
            if ( !this->currentState() || this->currentState() != county->state() )
            {
                this->m_currentCounty = county;
                this->setCurrentState( county->state() );
            }
            else
            {
                this->m_currentCounty = county;
                this->reloadData();
                Q_EMIT changed();
            }
        }
    }
}

/**
 * Возвращает список имен графств текущего штата
 *
 * @return  Объект типа QStringList
 * @author  Марунин А.В.
 * @since   2.8
 */
QStringList ddmBaseCountyFilterModel::countyNames() const
{
    return this->m_countyNames;
}

/**
 * Загружает информацию о заданном штате
 *
 * @param   state_id Идентификатор штата, который нужно загрузить
 * @author  Марунин А.В.
 * @since   2.3
 */
ddmState* ddmBaseCountyFilterModel::loadState( int state_id )
{
    ddmState* state = this->findState( state_id );
    if ( !state )
    {
        // Загружаем информацию о всех графствах штата
        QString sqlQuery = QString( "SELECT * FROM cache_boundaries WHERE state_id = %1" ).arg( state_id );
        ddmFilterModel* stateFilter = new ddmFilterModel;
        stateFilter->execQuery( sqlQuery );
        state = stateFilter->state( state_id );
        Q_ASSERT( state != NULL );

        // Передаем права на владение
        state->setParent( this );

        // Объединяем данные
        this->m_totalStates.unite( stateFilter->totalStates() );
        this->m_totalCounties.unite( stateFilter->totalCounties() );
        this->m_totalBoundaries.unite( stateFilter->totalBoundaries() );

        delete stateFilter;
    }

    return state;
}

/**
 * Обновляет список штатов
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmBaseCountyFilterModel::updateStateNames()
{
    this->m_stateMap.clear();

    ddmDatabase& db = this->database();
    QString sql = "SELECT state_id, state_name FROM cache_boundaries GROUP BY state_id";
    QSqlQueryModel* query = db.select( sql );
    Q_ASSERT( !db.hasErrors() );

    int rowCount = query->rowCount();
    for ( int i = 0; i < rowCount; i++ )
    {
        QSqlRecord record = query->record( i );
        int state_id = record.value( "state_id" ).toInt();
        QString state_name = record.value( "state_name" ).toString();
        this->m_stateMap.insert( state_id, state_name );
    }
}

/**
 * Обновляет список имен графств текущего штата
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmBaseCountyFilterModel::updateCountyNames()
{
    if ( this->currentState() )
    {
        this->m_countyNames.clear();
        QVariantList counties = this->currentState()->counties();
        foreach ( QVariant obj, counties )
        {
            ddmCounty* county = obj.value<ddmCounty*>();
            this->m_countyNames.append( county->geographicName() );
        }
    }
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
ddmBaseCountyFilterModel::~ddmBaseCountyFilterModel()
{
}
