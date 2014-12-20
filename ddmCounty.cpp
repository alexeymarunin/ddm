#include "ddmCounty.h"
#include "ddmModel.h"

ddmCounty::ddmCounty( int id, ddmModel* model ) : ddmObject( id, model ),
    m_state(0)
{
}

void ddmCounty::addBoundary( ddmBoundary* boundary )
{
    if ( boundary )
    {
        int boundary_id = boundary->id();
        this->m_boundaries.insert( boundary_id, boundary );
        boundary->setCounty( this );
    }
}

ddmBoundary* ddmCounty::boundary( int id ) const
{
    return this->m_boundaries[id];
}

ddmBoundaryMap& ddmCounty::boundaries() const
{
    return const_cast<ddmBoundaryMap&>( this->m_boundaries );
}

void ddmCounty::setState( ddmState* state )
{
    this->m_state = state;
}

ddmState* ddmCounty::state() const
{
    return this->m_state;
}


void ddmCounty::setCenter( const QPointF& point )
{
    this->m_center = point;
}

QPointF ddmCounty::center() const
{
    return this->m_center;
}

void ddmCounty::setGeographicName( const QString& name )
{
    this->m_geographicName = name;
}

QString ddmCounty::geographicName() const
{
    return this->m_geographicName;
}

void ddmCounty::setModel( ddmModel* model )
{
    this->setParent( model );
}

ddmModel* ddmCounty::model() const
{
    return dynamic_cast<ddmModel*>( this->parent() );
}

ddmCounty::~ddmCounty()
{
}

