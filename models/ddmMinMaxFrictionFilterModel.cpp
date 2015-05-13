#include "models/ddmMinMaxFrictionFilterModel.h"


/**
 * @brief ddmMinMaxFrictionFilterModel::ddmMinMaxFrictionFilterModel
 * @param parent
 */
ddmMinMaxFrictionFilterModel::ddmMinMaxFrictionFilterModel( QObject* parent ) : ddmFilterModel( parent )
{
    this->updateMinMaxFrictions();
    this->m_minBound = 0;
    this->m_maxBound = 0;
}

void ddmMinMaxFrictionFilterModel::reloadData()
{
    double minBound = this->minBound();
    double maxBound = this->maxBound();
    QString sqlQuery = QString( "SELECT * FROM cache_boundaries WHERE county_f_mid >= %1 AND county_f_mid <= %2" ).arg( minBound ).arg( maxBound );
    this->execQuery( sqlQuery );

}

double ddmMinMaxFrictionFilterModel::minBound() const
{
    return this->m_minBound;
}

void ddmMinMaxFrictionFilterModel::setMinBound( double bound )
{
    if ( this->minBound() != bound )
    {
        this->m_minBound = bound;
        this->reloadData();
        Q_EMIT changed();
    }
}

double ddmMinMaxFrictionFilterModel::maxBound() const
{
    return this->m_maxBound;
}

void ddmMinMaxFrictionFilterModel::setMaxBound( double bound )
{
    if ( this->maxBound() != bound )
    {
        this->m_maxBound = bound;
        this->reloadData();
        Q_EMIT changed();
    }
}

void ddmMinMaxFrictionFilterModel::setBounds( double minBound, double maxBound )
{
    if ( this->minBound() != minBound || this->maxBound() != maxBound )
    {
        this->m_minBound = minBound;
        this->m_maxBound = maxBound;
        this->reloadData();
        Q_EMIT changed();
    }
}

double ddmMinMaxFrictionFilterModel::minFriction() const
{
    return this->m_minFriction;
}

double ddmMinMaxFrictionFilterModel::maxFriction() const
{
    return this->m_maxFriction;
}

void ddmMinMaxFrictionFilterModel::updateMinMaxFrictions()
{
    using namespace std;
    this->m_minFriction = 0.0;
    this->m_maxFriction = 9.0e+9;
}

/**
 * @brief ddmMinMaxFrictionFilterModel::~ddmMinMaxFrictionFilterModel
 */
ddmMinMaxFrictionFilterModel::~ddmMinMaxFrictionFilterModel()
{
}

