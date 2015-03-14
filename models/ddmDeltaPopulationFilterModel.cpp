#include <models/ddmDeltaPopulationFilterModel.h>



ddmDeltaPopulationFilterModel::ddmDeltaPopulationFilterModel( QObject *parent ):
    ddmFilterModel( parent )
{
    m_minPopulatiom = 1.0e-5;
    m_maxPopulatiom = 1.0;
}


void ddmDeltaPopulationFilterModel::reloadData()
{
    double minBound = this->minBound() * 100; // переводим в проценты
    double maxBound = this->maxBound() * 100; // переводим в проценты
    QString sqlQuery = QString( "SELECT * FROM cache_boundaries WHERE county_delta >= county_population* 0.01 * %1 AND county_delta <= county_population* 0.01 * %2" ).arg( minBound ).arg( maxBound );
    this->execQuery( sqlQuery );



}


void ddmDeltaPopulationFilterModel::setMinBound( double bound )
{
    if ( this->minBound() != bound )
    {
        this->m_minBound = bound;
        this->reloadData();
        Q_EMIT changed();
    }
}


void ddmDeltaPopulationFilterModel::setMaxBound( double bound )
{
    if ( this->minBound() != bound )
    {
        this->m_maxBound = bound;
        this->reloadData();
        Q_EMIT changed();
    }
}


void ddmDeltaPopulationFilterModel::setBounds( double minBound, double maxBound )
{
    if ( this->minBound() != minBound || this->maxBound() != maxBound )
    {
        this->m_minBound = minBound;
        this->m_maxBound = maxBound;
        this->reloadData();
        Q_EMIT changed();
    }
}


double ddmDeltaPopulationFilterModel::maxBound() const
{
    return m_maxBound;
}


double ddmDeltaPopulationFilterModel::minBound() const
{
    return this->m_minBound;
}


double ddmDeltaPopulationFilterModel::minPopulation() const
{
    return m_minPopulatiom;
}


double ddmDeltaPopulationFilterModel::maxPopulation() const
{
    return m_maxPopulatiom;
}


ddmDeltaPopulationFilterModel::~ddmDeltaPopulationFilterModel()
{

}

