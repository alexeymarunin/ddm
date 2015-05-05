#include "base/ddmDefines.h"
#include "models/ddmPosNegDeltaFilterModel.h"


ddmPosNegDeltaFilterModel::ddmPosNegDeltaFilterModel( QObject* parent ) : ddmFilterModel( parent )
{
    this->m_deltaMode = DVM_INITIAL_STATE;
}

void ddmPosNegDeltaFilterModel::reloadData()
{
    int mode = this->deltaMode();
    QString sqlQuery;
    if ( mode == DVM_POSITIVE_DELTA )
        sqlQuery = QString( "SELECT * FROM cache_boundaries WHERE county_delta > 0" );
    else if ( mode == DVM_NEGATIVE_DELTA )
        sqlQuery = QString( "SELECT * FROM cache_boundaries WHERE county_delta < 0" );

    this->execQuery( sqlQuery );

}

void ddmPosNegDeltaFilterModel::setDeltaMode( int mode )
{
    if ( this->deltaMode() != mode )
    {
        this->m_deltaMode = mode;
        this->reloadData();
        Q_EMIT changed();
    }
}


int ddmPosNegDeltaFilterModel::deltaMode()
{
    return m_deltaMode;
}

ddmPosNegDeltaFilterModel::~ddmPosNegDeltaFilterModel()
{
}

