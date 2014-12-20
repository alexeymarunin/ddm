#include <ddmContainer.h>


ddmContainer::~ddmContainer()
{
    clear();
}


void ddmContainer::setName( QVariant name )
{
    m_name = name;
}

void ddmContainer::setId( QVariant id )
{
    m_id = id;
}


void ddmContainer::setCenter( QVariant center )
{
    m_center = center;
}


void ddmContainer::appendPoint( QVariant lat, QVariant lon )
{
    QString val = QObject::tr( "%1,%2" ).arg( lat.toDouble() ).arg( lon.toDouble() );
    m_points.append( val );
}


void ddmContainer::clear()
{
    m_points.clear();
    m_center.clear();
    m_id.clear();
    m_name.clear();
}

QVariant ddmContainer::id()
{
    return m_id;
}


QVariant ddmContainer::name()
{
    return m_name;
}


QVariant ddmContainer::center()
{
    return m_center;
}


QVariantList ddmContainer::points()
{
    return m_points;
}


int ddmContainer::length()
{
    return m_points.length();
}
