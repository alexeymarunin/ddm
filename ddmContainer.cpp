#include <ddmContainer.h>


ddmContainer::~ddmContainer()
{
    clear();
}


void ddmContainer::appendLat( QVariant lat )
{
    m_lat.append( lat );
}


void ddmContainer::appendLon( QVariant lon )
{
    m_lon.append( lon );
}


void ddmContainer::clear()
{
    m_lat.clear();
    m_lon.clear();
}


QVariantList ddmContainer::getLatList()
{
    return m_lat;
}


QVariantList ddmContainer::getLonList()
{
    return m_lon;
}


int ddmContainer::length()
{
    return m_lat.length();
}
