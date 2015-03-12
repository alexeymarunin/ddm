#include "models/ddmFrictionPopulationFilterModel.h"


ddmFrictionPopulationFilterModel::ddmFrictionPopulationFilterModel( QObject *parent ):
    ddmFilterModel( parent )
{
    this->updateMinMaxFrictions();
    this->updateMinMaxPopulation();

    this->m_minFrBound = 0.0;
    this->m_maxFrBound = 0.0;
    this->m_minPopBound = 0.0;
    this->m_maxPopBound = 0.0;
}


ddmFrictionPopulationFilterModel::~ddmFrictionPopulationFilterModel()
{

}


void ddmFrictionPopulationFilterModel::reloadData()
{
    int mult = 1000;
    double minFrBound = this->minFrBound();
    double maxFrBound = this->maxFrBound();
    int minPopBoun = (int) ( this->minPopBound() * mult );
    int maxPopBoun = (int) ( this->maxPopBound() * mult );
    QString sqlQuery = QString( "SELECT * FROM cache_boundaries WHERE county_f_mid >= %1 AND county_f_mid <= %2 AND county_population >= %3 AND county_population <= %4" ).arg( minFrBound ).arg( maxFrBound )
            .arg( minPopBoun ).arg( maxPopBoun );
    this->execQuery( sqlQuery );

}


void ddmFrictionPopulationFilterModel::setMinFrBound( double bound )
{
    if ( this->minFrBound() != bound )
    {
        this->m_minFrBound = bound;
        this->reloadData();
        Q_EMIT changed();
    }
}


void ddmFrictionPopulationFilterModel::setMaxFrBound( double bound )
{
    if ( this->maxFrBound() != bound )
    {
        this->m_maxFrBound = bound;
        this->reloadData();
        Q_EMIT changed();
    }
}


void ddmFrictionPopulationFilterModel::setFrBounds( double minBound, double maxBound )
{
    if ( this->minFrBound() != minBound || this->maxFrBound() != maxBound )
    {
        this->m_minFrBound = minBound;
        this->m_maxFrBound = maxBound;
        this->reloadData();
        Q_EMIT changed();
    }
}


void ddmFrictionPopulationFilterModel::setMinPopBound( double bound )
{
    if( minPopBound() != bound )
    {
        m_minPopBound = bound;
        this->reloadData();
        Q_EMIT changed();
    }
}


void ddmFrictionPopulationFilterModel::setMaxPopBound( double bound )
{
    if( maxPopBound() != bound )
    {
        m_maxPopBound = bound;
        this->reloadData();
        Q_EMIT changed();
    }
}


void ddmFrictionPopulationFilterModel::setPopBounds( double minBound, double maxBound )
{
    if ( this->minPopBound() != minBound || this->maxPopBound() != maxBound )
    {
        this->m_minPopBound = minBound;
        this->m_maxPopBound = maxBound;
        this->reloadData();
        Q_EMIT changed();
    }
}


void ddmFrictionPopulationFilterModel::updateMinMaxFrictions()
{
    ddmDatabase& db = this->database();
    QString sql = "SELECT MIN(f_mid) AS min_friction, MAX(f_mid) AS max_friction FROM ddm_frictions";
    QSqlQueryModel* query = db.select( sql );
    Q_ASSERT( !db.hasErrors() );

    QSqlRecord record = query->record( 0 );
    this->m_minFriction = record.value( "min_friction" ).toDouble();
    this->m_maxFriction = record.value( "max_friction" ).toDouble();

    qDebug() << "minFriction:" << this->m_minFriction;
    qDebug() << "maxFriction:" << this->m_maxFriction;
}


void ddmFrictionPopulationFilterModel::updateMinMaxPopulation()
{
    double mult = 0.001;
    ddmDatabase& db = this->database();
    QString sql = "SELECT MIN(popul_est) AS min_population, MAX(popul_est) AS max_population FROM ddm_residences";
    QSqlQueryModel* query = db.select( sql );
    Q_ASSERT( !db.hasErrors() );

    QSqlRecord record = query->record( 0 );
    this->m_minPopulation = record.value( "min_population" ).toDouble();
    this->m_maxPopulation = record.value( "max_population" ).toDouble();

    this->m_minPopulation = this->m_minPopulation * mult;
    this->m_maxPopulation = this->m_maxPopulation * mult;

    qDebug() << "minPopulation:" << this->m_minPopulation;
    qDebug() << "maxPopulation:" << this->m_maxPopulation;
}


double ddmFrictionPopulationFilterModel::minPopulation() const
{
    return m_minPopulation;
}


double ddmFrictionPopulationFilterModel::maxPopulation() const
{
    return m_maxPopulation;
}


double ddmFrictionPopulationFilterModel::maxPopBound() const
{
    return m_maxPopBound;
}


double ddmFrictionPopulationFilterModel::minFriction() const
{
    return m_minFriction;
}


double ddmFrictionPopulationFilterModel::maxFriction() const
{
    return m_maxFriction;
}


double ddmFrictionPopulationFilterModel::minPopBound() const
{
    return m_minPopBound;
}


double ddmFrictionPopulationFilterModel::minFrBound() const
{
    return this->m_minFrBound;
}


double ddmFrictionPopulationFilterModel::maxFrBound() const
{
    return this->m_maxFrBound;
}
