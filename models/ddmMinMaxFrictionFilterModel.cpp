#include "models/ddmMinMaxFrictionFilterModel.h"


/**
 * @brief ddmMinMaxFrictionFilterModel::ddmMinMaxFrictionFilterModel
 * @param parent
 */
ddmMinMaxFrictionFilterModel::ddmMinMaxFrictionFilterModel( QObject* parent ) : ddmFilterModel( parent )
{
}

void ddmMinMaxFrictionFilterModel::load()
{
    ddmFilterModel::load();

    ddmDatabase& db = this->database();
    QString sql = "SELECT MIN(f_mid) AS min_friction, MAX(f_mid) AS max_friction FROM ddm_frictions";
    QSqlQueryModel* query = db.select( sql );
    Q_ASSERT( !db.hasErrors() );

    QSqlRecord record = query->record( 0 );
    this->m_minFriction = record.value( "min_friction" ).toDouble();
    this->m_maxFriction = record.value( "max_friction" ).toDouble();
}

double ddmMinMaxFrictionFilterModel::minBound() const
{
    return this->m_minBound;
}

void ddmMinMaxFrictionFilterModel::setMinBound( double bound )
{
    this->m_minBound = bound;
    Q_EMIT changed();
}

double ddmMinMaxFrictionFilterModel::maxBound() const
{
    return this->m_maxBound;
}

void ddmMinMaxFrictionFilterModel::setMaxBound( double bound )
{
    this->m_maxBound = bound;
    Q_EMIT changed();
}

double ddmMinMaxFrictionFilterModel::minFriction() const
{
    return this->m_minFriction;
}

double ddmMinMaxFrictionFilterModel::maxFriction() const
{
    return this->m_maxFriction;
}

/**
 * @brief ddmMinMaxFrictionFilterModel::~ddmMinMaxFrictionFilterModel
 */
ddmMinMaxFrictionFilterModel::~ddmMinMaxFrictionFilterModel()
{
}

