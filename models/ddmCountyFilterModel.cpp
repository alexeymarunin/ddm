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
    this->load();
    this->updateStateNames();
    this->setCurrentState( this->states().first().value<ddmState*>() );
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
void ddmCountyFilterModel::setCurrentState( int id )
{
    ddmState* state = this->state( id );
    if ( state )
    {
        this->setCurrentState( state );
    }
}

/**
 * @brief ddmCountyFilterModel::setCurrentState
 * @param geographicName
 */
void ddmCountyFilterModel::setCurrentState( const QString& geographicName )
{
    ddmState* state = this->state( geographicName );
    if ( state )
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
            if ( !this->currentCounty() || this->currentCounty()->state()->id() != this->currentState()->id() )
            {
                this->updateCountyNames();
                QVariantList counties = state->counties();
                this->setCurrentCounty( counties.first().value<ddmCounty*>() );
            }
        }
    }
}

/**
 * @brief ddmCountyFilterModel::stateNames
 * @return
 */
QStringList ddmCountyFilterModel::stateNames() const
{
    return this->m_stateNames;
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
    ddmCounty* county = this->currentState()->county( id );
    if ( county )
    {
        this->setCurrentCounty( county );
    }
}

/**
 * @brief ddmCountyFilterModel::setCurrentCounty
 * @param geographicName
 */
void ddmCountyFilterModel::setCurrentCounty( const QString& geographicName )
{
    ddmCounty* county = this->currentState()->county( geographicName );
    if ( county )
    {
        this->setCurrentCounty( county );
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
 * @brief ddmCountyFilterModel::updateStateNames
 */
void ddmCountyFilterModel::updateStateNames()
{
    this->m_stateNames.clear();
    QVariantList states = this->states();
    foreach ( QVariant obj, states )
    {
        ddmState* state = obj.value<ddmState*>();
        this->m_stateNames.append( state->geographicName() );
    }
}

/**
 * @brief ddmCountyFilterModel::updateCountyNames
 */
void ddmCountyFilterModel::updateCountyNames()
{
    this->m_countyNames.clear();
    QVariantList counties = this->currentState()->counties();
    foreach ( QVariant obj, counties )
    {
        ddmCounty* county = obj.value<ddmCounty*>();
        this->m_countyNames.append( county->geographicName() );
    }
}

/**
 * @brief ddmCountyFilterModel::~ddmCountyFilterModel
 */
ddmCountyFilterModel::~ddmCountyFilterModel()
{
}
