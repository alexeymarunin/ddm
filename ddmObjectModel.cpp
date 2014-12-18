#include "ddmObjectModel.h"
#include "ddmContainer.h"

#include <QStringList>


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
    int state_id = GetCurentStateId();
    emit updateCounty( m_county, state_id );
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


int ddmObjectModel::GetCurentStateId()
{
    int state_id = -1;
    if( m_state.isEmpty() )
        return state_id;
    QSqlQuery query;
    QString text = QObject::tr( "SELECT id FROM ddm_states WHERE name = '%1'"  ).arg( m_state );
    query.exec( text );
    while( query.next() )
        state_id = query.value(0).toInt();
    return state_id;

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


QPointF ddmObjectModel::getCountyCenter( const QString &county )
{
    QPointF point;
    QSqlQuery query;
    int state_id = GetCurentStateId();
    QString text = QObject::tr( "SELECT id FROM ddm_counties WHERE title = '%1' AND state_id = %2"  ).arg( county ).arg( state_id );
    query.exec( text );
    int county_id = -1;
    while( query.next() )
        county_id = query.value(0).toInt();

    text = "SELECT p.x, p.y FROM ddm_county_boundaries AS cb LEFT JOIN ";
    text.append( "ddm_counties AS c ON c.id = cb.county_id " );
    text.append( "LEFT JOIN ddm_boundaries AS b ON b.id = cb.boundary_id " );
    text.append( "LEFT JOIN ddm_points AS p ON p.id = b.center_id " );
    text.append( QObject::tr( "WHERE c.id = %1 AND b.outer = 1 ORDER BY b.square DESC LIMIT 1" ).arg( county_id ) );
    query.exec( text );
    while( query.next() )
    {
        point.setX( query.value(0).toDouble() );
        point.setY( query.value(1).toDouble() );
    }

    return point;
}


void ddmObjectModel::getCountyBoundary( const QString &county, ddmContainer* boundary )
{
   if( county.isEmpty() || boundary == NULL )
       return;

   int state_id = GetCurentStateId();
   if( state_id == -1 )
       return;

   QSqlQuery query;
   QString text = QObject::tr( "SELECT id FROM ddm_counties WHERE title = '%1' AND state_id = %2"  ).arg( county ).arg( state_id );
   query.exec( text );
   int county_id = -1;
   while( query.next() )
       county_id = query.value(0).toInt();

   if( county_id == -1 )
       return;

   text = "SELECT b.id FROM ddm_county_boundaries AS cb LEFT JOIN ";
   text.append( "ddm_boundaries AS b ON b.id = cb.boundary_id " );
   text.append( QObject::tr( "WHERE b.outer = 1 AND cb.county_id = %1 " ).arg( county_id ) );
   text.append( "ORDER BY b.square DESC LIMIT 1" );
   query.exec( text );
   int boundary_id = -1;
   while( query.next() )
       boundary_id = query.value(0).toInt();

   if( boundary_id == -1 )
       return;

   text = "SELECT p.x, p.y FROM ddm_boundary_points AS bp LEFT JOIN ";
   text.append( QObject::tr( "ddm_points AS p ON p.id = bp.point_id WHERE bp.boundary_id = %1" ).arg( boundary_id ) );
   query.exec( text );
   while( query.next() )
   {
       double x = query.value( 0 ).toDouble();
       double y = query.value( 1 ).toDouble();
       boundary->appendLon( x );
       boundary->appendLat( y );
   }
}
