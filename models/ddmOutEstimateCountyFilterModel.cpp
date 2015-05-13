#include "models/ddmOutEstimateCountyFilterModel.h"

#include <limits>



ddmOutEstimateCountyFilterModel::ddmOutEstimateCountyFilterModel( QObject *parent ) : ddmBaseCountyFilterModel( parent )
{
    this->updateMinMaxPopulation();
    this->m_minPopBound = 0.001;
    this->m_maxPopBound = 0.1;
}


void ddmOutEstimateCountyFilterModel::reloadData()
{
    if ( this->currentState() )
    {
        this->loadState( this->currentState()->id() );
        this->updateCountyNames();
        if ( this->currentCounty() )
        {
            this->updateCounties();


            QStringList counties;
            counties.append( QString( "%1" ).arg( this->currentCounty()->id() ) );
            foreach ( int county_id, this->m_counties )
                counties.append( QString( "%1" ).arg( county_id ) );

            QString sqlQuery = QString( "SELECT * FROM cache_boundaries AS cb WHERE county_id  IN (%1)" ).arg( counties.join( ',' ) );
            this->execQuery( sqlQuery );
        }
    }
}


double ddmOutEstimateCountyFilterModel::minPopBound() const
{
    return this->m_minPopBound;
}


void ddmOutEstimateCountyFilterModel::setMinPopBound( double bound )
{
    if( this->minPopBound() != bound )
    {
        m_minPopBound = bound;
        this->reloadData();
        Q_EMIT changed();
    }
}


double ddmOutEstimateCountyFilterModel::maxPopBound() const
{
    return m_maxPopBound;
}


void ddmOutEstimateCountyFilterModel::setMaxPopBound( double bound )
{
    if( this->maxPopBound() != bound )
    {
        m_maxPopBound = bound;
        this->reloadData();
        Q_EMIT changed();
    }
}


void ddmOutEstimateCountyFilterModel::setPopBounds( double minBound, double maxBound )
{
    if ( this->minPopBound() != minBound || this->maxPopBound() != maxBound )
    {
        this->m_minPopBound = minBound;
        this->m_maxPopBound = maxBound;
        this->reloadData();
        Q_EMIT changed();
    }
}


double ddmOutEstimateCountyFilterModel::minPopulation() const
{
    return m_minPopulation;
}


double ddmOutEstimateCountyFilterModel::maxPopulation() const
{
    return m_maxPopulation;
}


QSet<int> ddmOutEstimateCountyFilterModel::counties() const
{
    return m_counties;
}


void ddmOutEstimateCountyFilterModel::updateCounties()
{
    this->m_counties.clear();
    if ( this->currentCounty() )
    {
        ddmDatabase& db = this->database();
        QString sql;
        QSqlQueryModel* query;
        int i, rowCount;

        int mult = 1000;
        int minPopBoun = (int) ( this->minPopBound() * mult );
        int maxPopBoun = (int) ( this->maxPopBound() * mult );
        sql = QString( "SELECT county_b FROM ddm_mirgations WHERE out_estimate >= %1 AND out_estimate <= %2 AND county_a = %3" ).arg( minPopBoun ).arg( maxPopBoun ).arg( this->currentCounty()->id() );
        query = db.select( sql );
        Q_ASSERT( db.hasErrors() == false );
        rowCount = query->rowCount();
        for ( i = 0; i < rowCount; i++ )
        {
            int county_id = query->data( query->index( i, 0 ) ).toInt();
            this->m_counties.insert( county_id );
        }
    }

}


void ddmOutEstimateCountyFilterModel::updateMinMaxPopulation()
{
    using namespace std;
    this->m_minPopulation = 0.0;
    this->m_maxPopulation = 9.0e+9;
}


ddmOutEstimateCountyFilterModel::~ddmOutEstimateCountyFilterModel()
{

}
