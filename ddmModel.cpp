#include "ddmModel.h"

#include <QMessageBox>


ddmModel::ddmModel( QObject* parent ) : QObject( parent ),
    m_currentState( 0 ),
    m_currentCounty( 0 )
{
}

bool ddmModel::openDatabase( const QString& path )
{
    this->m_db = QSqlDatabase::addDatabase( "QSQLITE" );
    this->m_db.setDatabaseName( path );
    if ( this->m_db.open() )
    {
        this->loadStates();
        this->loadCounties();
        this->loadBoundaries();

        this->setCurrentState( this->states().first() );

        return true;
    }

    return false;
}

void ddmModel::loadStates()
{
    if ( this->m_states.empty() )
    {
        QString sql = QObject::tr( "SELECT s.id, s.name FROM ddm_frictions AS f\n" );
        sql.append( "LEFT JOIN ddm_counties AS c ON c.id = f.county_id\n" );
        sql.append( "LEFT JOIN ddm_states AS s ON s.id = c.state_id\n" );
        sql.append( "GROUP BY s.id\n" );
        sql.append( "ORDER BY s.id\n" );

        QSqlQuery query;
        query.exec( sql );
        while ( query.next() )
        {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();
            ddmState* state = new ddmState( id, this );
            state->setGeographicName( name );
            this->m_states.insert( id, state );
            QList<ddmCounty*> emplyList;
            this->m_stateCounties.insert( state, emplyList );
        }
    }
}

void ddmModel::loadCounties()
{
    if ( !this->m_states.empty() && this->m_counties.empty() )
    {
        QSqlQuery query;
        QString sql = QObject::tr( "SELECT c.id, c.geographic_name, c.state_id, p.x, p.y FROM ddm_frictions AS f\n" );
        sql.append( "LEFT JOIN ddm_counties AS c ON c.id = f.county_id\n" );
        sql.append( "LEFT JOIN ddm_points AS p ON p.id = c.center_id\n" );
        sql.append( "GROUP BY c.id\n" );
        sql.append( "ORDER BY c.id\n" );

        query.exec( sql );
        while ( query.next() )
        {
            int id = query.value(0).toInt();
            QString geographic_name = query.value(1).toString();
            int state_id = query.value(2).toInt();
            double x = query.value(3).toDouble();
            double y = query.value(4).toDouble();
            QPointF center( x, y );
            ddmCounty* county = new ddmCounty( id, this );
            county->setGeographicName( geographic_name );
            county->setCenter( center );
            ddmState* state = this->m_states[state_id];
            county->setState( state );
            this->m_counties.insert( id, county );
            this->m_stateCounties[state].append( county );
        }
    }
}

void ddmModel::loadBoundaries()
{
    if ( !this->m_counties.empty() && this->counties().first()->boundaries().empty() )
    {
        ddmBoundaryMap boundaries;
        do {
            QString sql = QObject::tr( "SELECT b.id, b.square, p.x, p.y FROM ddm_boundary_points AS bp\n" );
            sql.append( "LEFT JOIN ddm_boundaries AS b ON b.id = bp.boundary_id\n" );
            sql.append( "LEFT JOIN ddm_points AS p ON p.id = bp.point_id\n" );
            sql.append( "WHERE b.outer = 1\n" );
            sql.append( "ORDER BY b.id\n" );

            ddmBoundary* boundary = NULL;
            ddmBoundary* prevBoundary = NULL;
            QSqlQuery query;
            query.exec( sql );
            while ( query.next() )
            {
                int id = query.value(0).toInt();
                double square = query.value(1).toDouble();
                double x = query.value(2).toDouble();
                double y = query.value(3).toDouble();
                QPointF point( x, y );
                if ( !prevBoundary || id != prevBoundary->id() ) {
                    boundary = new ddmBoundary( id );
                    boundary->setSquare( square );
                    boundaries.insert( id, boundary );
                    prevBoundary = boundary;
                }
                boundary->addVertex( point );
            }

        } while( 0 );

        if ( !boundaries.empty() )
        {
            QString sql = QObject::tr( "SELECT cb.county_id, cb.boundary_id, p.x, p.y FROM ddm_county_boundaries AS cb\n" );
            sql.append( "LEFT JOIN ddm_boundaries AS b ON b.id = cb.boundary_id\n" );
            sql.append( "LEFT JOIN ddm_points AS p ON p.id = b.center_id\n" );
            sql.append( "WHERE b.outer = 1\n" );
            sql.append( "ORDER BY cb.county_id, b.square DESC\n" );

            ddmCounty* county = NULL;
            ddmCounty* prevCounty = NULL;
            QSqlQuery query;
            query.exec( sql );
            while ( query.next() )
            {
                int county_id = query.value(0).toInt();
                if ( !prevCounty || county_id != county->id() ) {
                    if ( !this->m_counties.contains( county_id ) ) continue;
                    int boundary_id = query.value(1).toInt();
                    double x = query.value(2).toDouble();
                    double y = query.value(3).toDouble();
                    QPointF center( x, y );
                    ddmBoundary* boundary = boundaries[boundary_id];
                    county = this->m_counties[county_id];
                    county->setCenter( center );
                    county->addBoundary( boundary );
                    boundary->setCounty( county );
                    prevCounty = county;
                }
            }
        }
    }
}

ddmStateMap& ddmModel::states() const
{
    return const_cast<ddmStateMap&>( this->m_states );
}

ddmCountyMap& ddmModel::counties() const
{
    return const_cast<ddmCountyMap&>( this->m_counties );
}

ddmState* ddmModel::currentState() const
{
    return this->m_currentState;
}

ddmCounty* ddmModel::currentCounty() const
{
    return this->m_currentCounty;
}

void ddmModel::setCurrentState( int id )
{
    this->setCurrentState( this->m_states[id] );
}

void ddmModel::setCurrentState( const QString& stateName )
{
    foreach ( ddmState* state, this->states() )
    {
        QString name = state->geographicName();
        if ( name.contains( stateName, Qt::CaseInsensitive ) ) {
            this->setCurrentState( state );
            return;
        }
    }
}

void ddmModel::setCurrentState( ddmState* state )
{
    if ( state != this->m_currentState )
    {
        this->m_currentState = state;
        emit changedState( state );
        if ( !this->currentCounty() || this->currentCounty()->state() != state )
        {
            ddmCounty* firstCounty = this->m_stateCounties[state].first();
            this->setCurrentCounty( firstCounty );
        }
    }
}

void ddmModel::setCurrentCounty( int id )
{
    this->setCurrentCounty( this->m_counties[id] );
}

void ddmModel::setCurrentCounty( const QString& countyName )
{
    foreach ( ddmCounty* county, this->counties() )
    {
        QString name = county->geographicName();
        if ( name.contains( countyName, Qt::CaseInsensitive ) ) {
            this->setCurrentCounty( county );
            return;
        }
    }
}

void ddmModel::setCurrentCounty( ddmCounty* county )
{
    if ( county != this->m_currentCounty )
    {
        this->setCurrentState( county->state() );
        this->m_currentCounty = county;
        emit changedCounty( county );
    }
}

QStringList ddmModel::stateNames() const
{
    QStringList names;
    foreach ( ddmState* state, this->states() )
    {
        QString name = state->geographicName();
        names.append( name );
    }
    return names;
}

QStringList ddmModel::countyNames() const
{
    QStringList names;
    ddmState* state = this->currentState();
    QList<ddmCounty*> counties = this->m_stateCounties[state];
    foreach ( ddmCounty* county, counties )
    {
        QString name = county->geographicName();
        names.append( name );
    }
    return names;
}

QVariantMap ddmModel::getProperties() const
{
    QVariantMap result;

    result.insert( "currentCounty", this->currentCounty()->id() );

    QVariantList counties;
    foreach ( ddmCounty* county, this->counties() )
    {
        counties.append( county->getProperties() );
    }
    result.insert( "counties", counties );

    return result;
}


void ddmModel::setCurrentCountyFromJS( int countyId )
{
    setCurrentCounty( countyId );
}


void ddmModel::getCurrentCoordsFromJS( const QString& lat, const QString& lng )
{
    emit changedCoords( lat, lng );
}


ddmModel::~ddmModel()
{
    m_db.close();
}

