#include <ddmObjectModel.h>


ddmObjectModel::ddmObjectModel()
{

}


ddmObjectModel::~ddmObjectModel()
{
    m_dbase.close();
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

bool ddmObjectModel::openDB( const QString &fullPath )
{
    QSqlDatabase dbase = QSqlDatabase::addDatabase( "QSQLITE" );
    dbase.setDatabaseName( fullPath );
    return dbase.open();
}


QString ddmObjectModel::CurentState()
{
    return m_state;
}


QString ddmObjectModel::CurentCounty()
{
    return m_county;
}


bool ddmObjectModel::isOpen()
{
    return m_dbase.isOpen();
}


void ddmObjectModel::getStatesList( QStringList& states )
{
    states.clear();
    QSqlQuery query;
    query.exec( "SELECT name FROM ddm_states" );
    while( query.next() )
    {
        QString state = query.value(0).toString();
        states.append( state );
    }
}


void ddmObjectModel::getCountiesList( QStringList& counties, const QString& state )
{
    counties.clear();
    QSqlQuery query;
    QString text = QObject::tr( "SELECT id FROM ddm_states WHERE name = '%1'"  ).arg( state );
    query.exec( text );
    int state_id = -1;
    while( query.next() )
        state_id = query.value(0).toInt();
    text = QObject::tr( "SELECT title FROM ddm_counties WHERE state_id = %1 " ).arg( state_id );
    query.exec( text );
    while( query.next() )
    {
        QString county = query.value(0).toString();
        counties.append( county );
    }
}
