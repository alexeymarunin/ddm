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
    emit updateState( m_state );
}

void ddmObjectModel::setCurentCounty( const QString county )
{
    m_county = county;
    emit updateCounty( m_county );
}


QString ddmObjectModel::CurentState()
{
    return m_state;
}


QString ddmObjectModel::CurentCounty()
{
    return m_county;
}
