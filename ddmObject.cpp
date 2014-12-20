#include "ddmObject.h"

ddmObject::ddmObject( int id, QObject* parent ) : QObject( parent ), m_id( id )
{
}

void ddmObject::setId( int id )
{
    this->m_id = id;
}

int ddmObject::id() const
{
    return this->m_id;
}

ddmObject::~ddmObject()
{
}

