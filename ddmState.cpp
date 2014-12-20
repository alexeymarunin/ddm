#include "ddmState.h"
#include "ddmModel.h"

ddmState::ddmState( int id, ddmModel* model ) : ddmObject( id, model )
{
}

void ddmState::setGeographicName( const QString& name )
{
    this->m_geographicName = name;
}

QString ddmState::geographicName() const
{
    return this->m_geographicName;
}

ddmState::~ddmState()
{
}


