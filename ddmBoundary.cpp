#include "ddmBoundary.h"
#include "ddmCounty.h"

ddmBoundary::ddmBoundary( int id, ddmCounty* county ) : ddmObject( id, county ),
    m_square(0)
{
}

void ddmBoundary::addVertex( const QPointF& point )
{
    this->m_vertices.append( point );
}

QPointF ddmBoundary::vertex( int id ) const
{
    return this->m_vertices[id];
}

ddmPointList& ddmBoundary::vertices() const
{
    return const_cast<ddmPointList&>( this->m_vertices );
}

void ddmBoundary::setCounty( ddmCounty* county )
{
    this->setParent( county );
}

ddmCounty* ddmBoundary::county() const
{
    return dynamic_cast<ddmCounty*>( this->parent() );
}

void ddmBoundary::setSquare( double square )
{
    this->m_square = square;
}

double ddmBoundary::square() const
{
    return this->m_square;
}

void ddmBoundary::setCenter( const QPointF& point )
{
    this->m_center = point;
}

QPointF ddmBoundary::center() const
{
    return this->m_center;
}

QVariantMap ddmBoundary::getProperties() const
{
    QVariantMap result;

    result.insert( "id", this->id() );

    QVariantMap center;
    center.insert( "x", this->center().x() );
    center.insert( "y", this->center().y() );
    result.insert( "center", center );
    result.insert( "square", this->square() );

    QVariantList vertices;
    foreach ( QPointF vertex, this->vertices() )
    {
        QVariantMap point;
        point.insert( "x", vertex.x() );
        point.insert( "y", vertex.y() );
        vertices.append( point );
    }
    result.insert( "vertices", vertices );

    return result;
}

ddmBoundary::~ddmBoundary()
{
}

