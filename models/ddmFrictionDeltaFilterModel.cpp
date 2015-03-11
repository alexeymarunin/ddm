#include "base/ddmDefines.h"
#include "ddmFrictionDeltaFilterModel.h"


ddmFrictionDeltaFilterModel::ddmFrictionDeltaFilterModel( QObject* parent ) : ddmFilterModel( parent )
{
    this->updateMinMaxFrictions();
    this->m_deltaMode = 0;
}

void ddmFrictionDeltaFilterModel::reloadData()
{
    double minBound = this->minBound();
    double maxBound = this->maxBound();
    int mode = this->deltaMode();
    QString sqlQuery = QString( "SELECT * FROM cache_boundaries WHERE county_f_mid >= %1 AND county_f_mid <= %2" ).arg( minBound ).arg( maxBound );
    if( mode == DVM_POSITIVE_DELTA  )
        sqlQuery = QString(  "%1 AND county_delta > 0" ).arg( sqlQuery );
    else if ( mode == DVM_NEGATIVE_DELTA )
        sqlQuery = QString(  "%1 AND county_delta < 0" ).arg( sqlQuery );
    this->execQuery( sqlQuery );
}


double ddmFrictionDeltaFilterModel::minBound() const
{
    return this->m_minBound;
}


void ddmFrictionDeltaFilterModel::setMinBound( double bound )
{
    if ( this->minBound() != bound )
    {
        this->m_minBound = bound;
        this->reloadData();
        Q_EMIT changed();
    }
}


double ddmFrictionDeltaFilterModel::maxBound() const
{
    return this->m_maxBound;
}


void ddmFrictionDeltaFilterModel::setMaxBound(double bound)
{
    if ( this->maxBound() != bound )
    {
        this->m_maxBound = bound;
        this->reloadData();
        Q_EMIT changed();
    }
}


void ddmFrictionDeltaFilterModel::setBounds( double minBound, double maxBound )
{
    if ( this->minBound() != minBound || this->maxBound() != maxBound )
    {
        this->m_minBound = minBound;
        this->m_maxBound = maxBound;
        this->reloadData();
        Q_EMIT changed();
    }
}


double ddmFrictionDeltaFilterModel::minFriction() const
{
    return this->m_minFriction;
}


double ddmFrictionDeltaFilterModel::maxFriction() const
{
    return this->m_maxFriction;

}


void ddmFrictionDeltaFilterModel::setDeltaMode( int mode )
{
    // Тарас, обязательно проверяй, что изменения действительно есть!
    // Иначе:
    //  а) будут вызываться ненужные обработки
    //  б) рискуешь попасть в бесконечный цикл
    if ( this->deltaMode() != mode )
    {
        m_deltaMode = mode;
        this->reloadData();
        Q_EMIT changed();
    }
}


int ddmFrictionDeltaFilterModel::deltaMode()
{
    return this->m_deltaMode;
}

void ddmFrictionDeltaFilterModel::updateMinMaxFrictions()
{
    ddmDatabase& db = this->database();
    QString sql = "SELECT MIN(f_mid) AS min_friction, MAX(f_mid) AS max_friction FROM ddm_frictions";
    QSqlQueryModel* query = db.select( sql );
    Q_ASSERT( !db.hasErrors() );

    this->m_minFriction = query->data( query->index( 0, 0 ) ).toDouble();
    this->m_maxFriction = query->data( query->index( 0, 1 ) ).toDouble();
}

ddmFrictionDeltaFilterModel::~ddmFrictionDeltaFilterModel()
{
}

