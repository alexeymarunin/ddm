#include <QSqlRecord>
#include <QtDebug>

#include "models/ddmCountyFilterModel.h"

/**
 * @brief ddmCountyFilterModel::ddmCountyFilterModel
 * @param parent
 */
ddmCountyFilterModel::ddmCountyFilterModel( QObject* parent ) : ddmFilterModel( parent ),
    m_currentState( NULL ), m_currentCounty( NULL )
{
    this->updateStateNames();
    this->setCurrentState( "Alabama" );
}

/**
 * Загружает данные из БД
 *
 * @author  Марунин А.В.
 * @since   2.3
 */
void ddmCountyFilterModel::reloadData()
{
    if ( this->currentState() )
    {
        // Выбираем графства текущего штата
        this->loadState( this->currentState()->id() );
        this->updateCountyNames();
    }
}

/**
 * @brief ddmCountyFilterModel::currentState
 * @return
 */
ddmState* ddmCountyFilterModel::currentState() const
{
    return this->m_currentState;
}

/**
 * @brief ddmCountyFilterModel::setCurrentState
 * @param id
 */
void ddmCountyFilterModel::setCurrentState( int state_id )
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
 * @brief ddmCountyFilterModel::setCurrentState
 * @param geographicName
 */
void ddmCountyFilterModel::setCurrentState( const QString& geographicName )
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
 * @brief ddmCountyFilterModel::setCurrentState
 * @param state
 */
void ddmCountyFilterModel::setCurrentState( ddmState* state )
{
    if ( state )
    {
        if ( this->m_currentState != state )
        {
            this->m_currentState = state;
            this->reloadData();
            QVariantList counties = state->counties();
            this->setCurrentCounty( counties.first().value<ddmCounty*>() );
        }
    }
}

/**
 * @brief ddmCountyFilterModel::stateNames
 * @return
 */
QStringList ddmCountyFilterModel::stateNames() const
{
    return QStringList( this->m_stateMap.values() );
}

/**
 * @brief ddmCountyFilterModel::currentCounty
 * @return
 */
ddmCounty* ddmCountyFilterModel::currentCounty() const
{
    return this->m_currentCounty;
}

/**
 * @brief ddmCountyFilterModel::setCurrentCounty
 * @param id
 */
void ddmCountyFilterModel::setCurrentCounty( int id )
{
    if ( this->currentState() )
    {
        ddmCounty* county = this->currentState()->county( id );
        if ( county )
        {
            this->setCurrentCounty( county );
        }
    }
}

/**
 * @brief ddmCountyFilterModel::setCurrentCounty
 * @param geographicName
 */
void ddmCountyFilterModel::setCurrentCounty( const QString& geographicName )
{
    if ( this->currentState() )
    {
        ddmCounty* county = this->currentState()->county( geographicName );
        if ( county )
        {
            this->setCurrentCounty( county );
        }
    }
}

/**
 * @brief ddmCountyFilterModel::setCurrentCounty
 * @param county
 */
void ddmCountyFilterModel::setCurrentCounty( ddmCounty* county )
{
    if ( county )
    {
        if ( this->m_currentCounty != county )
        {
            this->m_currentCounty = county;
            // Делаем текущим штат выбранного графства
            this->setCurrentState( county->state() );
            Q_EMIT changed();
        }
    }
}

/**
 * @brief ddmCountyFilterModel::countyNames
 * @return
 */
QStringList ddmCountyFilterModel::countyNames() const
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
ddmState* ddmCountyFilterModel::loadState( int state_id )
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
 * @brief ddmCountyFilterModel::updateStateNames
 */
void ddmCountyFilterModel::updateStateNames()
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
 * @brief ddmCountyFilterModel::updateCountyNames
 */
void ddmCountyFilterModel::updateCountyNames()
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
 * @brief ddmCountyFilterModel::~ddmCountyFilterModel
 */
ddmCountyFilterModel::~ddmCountyFilterModel()
{
}
