#include "models/ddmInEstimAndPopulFilterModel.h"


ddmInEstimAndPopulFilterModel::ddmInEstimAndPopulFilterModel( QObject *parent ):
    ddmBaseCountyFilterModel( parent )
{
    this->m_minPopBound = 1.0;
    this->m_maxPopBound = 5.0;
}


void ddmInEstimAndPopulFilterModel::reloadData()
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


double ddmInEstimAndPopulFilterModel::minPopBound() const
{
    return this->m_minPopBound;
}


void ddmInEstimAndPopulFilterModel::setMinPopBound( double bound )
{
    if( this->minPopBound() != bound )
    {
        m_minPopBound = bound;
        this->reloadData();
        Q_EMIT changed();
    }
}


double ddmInEstimAndPopulFilterModel::maxPopBound() const
{
    return m_maxPopBound;
}


void ddmInEstimAndPopulFilterModel::setMaxPopBound( double bound )
{
    if( this->maxPopBound() != bound )
    {
        m_maxPopBound = bound;
        this->reloadData();
        Q_EMIT changed();
    }
}


void ddmInEstimAndPopulFilterModel::setPopBounds( double minBound, double maxBound )
{
    if ( this->minPopBound() != minBound || this->maxPopBound() != maxBound )
    {
        this->m_minPopBound = minBound;
        this->m_maxPopBound = maxBound;
        this->reloadData();
        Q_EMIT changed();
    }
}


double ddmInEstimAndPopulFilterModel::minPopulation() const
{
    return 0.000; // исчисляется в процентах
}


double ddmInEstimAndPopulFilterModel::maxPopulation() const
{
    return 100; // исчисляется в процентах
}


QSet<int> ddmInEstimAndPopulFilterModel::counties() const
{
    return m_counties;
}


void ddmInEstimAndPopulFilterModel::updateCounties()
{
    this->m_counties.clear();
    if ( this->currentCounty() )
    {
        ddmDatabase& db = this->database();
        QString sql;
        QSqlQueryModel* query;
        int i, rowCount;

        double mult = 0.01;
        double minPopBound =  ( this->minPopBound() * mult );
        double maxPopBound =  ( this->maxPopBound() * mult );
        sql = QString( "SELECT popul_est FROM ddm_residences WHERE county_id = %1" ).arg( this->currentCounty()->id() );
        query = db.select( sql );
        Q_ASSERT( db.hasErrors() == false );
        Q_ASSERT( query->rowCount() == 1 );


        qlonglong population = query->data( query->index( 0, 0 ) ).toLongLong();
        qlonglong minPopulation = (qlonglong) population * minPopBound;
        qlonglong maxPopulation = (qlonglong) population * maxPopBound;
        sql = QString( "SELECT county_b FROM ddm_mirgations WHERE in_estimate >= %1 AND in_estimate <= %2 AND county_a = %3" )
                .arg( minPopulation ).arg( maxPopulation ).arg( this->currentCounty()->id() );
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


qlonglong ddmInEstimAndPopulFilterModel::getCurentCountyPopulation()
{
    if ( this->currentCounty() )
    {
        ddmDatabase& db = this->database();
        QString sql;
        QSqlQueryModel* query;

        sql = QString( "SELECT popul_est FROM ddm_residences WHERE county_id = %1" ).arg( this->currentCounty()->id() );
        query = db.select( sql );
        Q_ASSERT( db.hasErrors() == false );
        Q_ASSERT( query->rowCount() == 1 );
        qlonglong population = query->data( query->index( 0, 0 ) ).toLongLong();
        return population;
    }
    return -1;
}


ddmInEstimAndPopulFilterModel::~ddmInEstimAndPopulFilterModel()
{

}

