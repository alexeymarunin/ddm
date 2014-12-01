#include<ddmObjectModel.h>


ddmObjectModel::ddmObjectModel()
{

}


ddmObjectModel::~ddmObjectModel()
{

}


void ddmObjectModel::setCurentState( const QString state )
{
    m_state = state;
}

void ddmObjectModel::setCurentCounty( const QString county )
{
    m_county = county;
}


QString ddmObjectModel::CurentState()
{
    return m_state;
}


QString ddmObjectModel::CurentCounty()
{
    return m_county;
}
