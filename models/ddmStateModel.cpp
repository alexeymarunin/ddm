#include <QtDebug>
#include <QTime>
#include <QMap>

#include "models/ddmStateModel.h"
#include "base/ddmState.h"
#include "base/ddmCounty.h"
#include "base/ddmBoundary.h"

/**
 * @brief ddmStateModel::ddmStateModel
 * @param parent
 */
ddmStateModel::ddmStateModel( QObject* parent ) : ddmModel( parent )
{
}

void ddmStateModel::load()
{
    if ( this->stateCount() > 0 ) return;

    ddmDatabase& db = this->database();

    QString sql;
    QSqlQueryModel* query;
    int rowCount;

    QTime timer;
    timer.start();

    sql =
        "CREATE TABLE IF NOT EXISTS cache_boundaries AS\n"
        "  SELECT s.id AS state_id, s.name AS state_name,\n"
        "      c.id AS county_id, c.geographic_name AS county_name,\n"
        "      b.id AS boundary_id, p.x AS center_x, p.y center_y, b.square AS boundary_square,\n"
        "      f.f_out_sum AS county_f_out_sum, f.f_out_mid AS county_f_out_mid,\n"
        "      f.f_in_sum AS county_f_in_sum, f.f_in_mid AS county_f_in_mid,\n"
        "      f.f_mid AS county_f_mid \n"
        "    FROM ddm_county_boundaries AS cb\n"
        "    LEFT JOIN ddm_boundaries AS b ON b.id = cb.boundary_id\n"
        "    LEFT JOIN ddm_points AS p ON p.id = b.center_id\n"
        "    LEFT JOIN ddm_frictions AS f ON f.county_id = cb.county_id\n"
        "    LEFT JOIN ddm_counties AS c ON c.id = f.county_id\n"
        "    LEFT JOIN ddm_states AS s ON s.id = c.state_id\n"
        "    WHERE b.outer = 1\n"
        "    ORDER BY s.id, c.id, b.square DESC";

    db.exec( sql );
    Q_ASSERT( !db.hasErrors() );

    sql = "SELECT * FROM cache_boundaries";
    query = db.select( sql );
    Q_ASSERT( !db.hasErrors() );

    QMap<int, ddmBoundary*> boundaryMap;

    ddmState* currentState = NULL;
    ddmCounty* currentCounty = NULL;
    ddmBoundary* currentBoundary = NULL;
    rowCount = query->rowCount();
    for ( int i = 0; i < rowCount; i++ )
    {
        QSqlRecord record = query->record( i );
        int state_id = record.value( "state_id" ).toInt();
        if ( !state_id ) continue;

        if ( !currentState || state_id != currentState->id() )
        {
            ddmState* state = new ddmState( record );
            this->addState( state );
            currentState = state;
        }

        int county_id = record.value( "county_id" ).toInt();
        Q_ASSERT( county_id > 0 );
        if ( !currentCounty || county_id != currentCounty->id() )
        {
            ddmCounty* county = new ddmCounty( record );
            this->addCounty( county, currentState );
            currentCounty = county;
        }

        int boundary_id = record.value( "boundary_id" ).toInt();
        Q_ASSERT( boundary_id > 0 );
        if ( !currentBoundary || boundary_id != currentBoundary->id() )
        {
            ddmBoundary* boundary = new ddmBoundary( record );
            this->addBoundary( boundary, currentCounty );
            currentBoundary = boundary;
            boundaryMap.insert( boundary_id, boundary );
        }
    }

    sql =
        "CREATE TABLE IF NOT EXISTS cache_boundary_points AS\n"
        "  SELECT bp.boundary_id, p.x, p.y\n"
        "    FROM ddm_boundary_points AS bp\n"
        "    LEFT JOIN ddm_points AS p ON p.id = bp.point_id\n";
    db.exec( sql );
    Q_ASSERT( !db.hasErrors() );

    sql = "SELECT * FROM cache_boundary_points";
    query = db.select( sql );
    Q_ASSERT( !db.hasErrors() );

    rowCount = query->rowCount();
    for ( int i = 0; i < rowCount; i++ )
    {
        QSqlRecord record = query->record( i );
        int boundary_id = record.value( "boundary_id" ).toInt();
        QMap<int, ddmBoundary*>::iterator it = boundaryMap.find( boundary_id );
        if ( it != boundaryMap.end() )
        {
            ddmBoundary* boundary = it.value();
            this->addVertex( record, boundary );
        }
    }

    qDebug() << "Elapsed:" << timer.elapsed() / 1000.0 << "sec";
    //qDebug( "Found %d vertices on %d boundaries in %d counties for %d states\n", this->vertexCount(), this->boundaryCount(), this->countyCount(), this->stateCount() );

    return;
}

void ddmStateModel::addState( ddmState* state )
{
    state->setParent( this );
    this->m_states.append( QVariant::fromValue( state ) );
}

void ddmStateModel::addCounty( ddmCounty* county, ddmState* state )
{
    state->addCounty( county );
    this->m_counties.append( QVariant::fromValue( county ) );

    connect( county, SIGNAL( clicked(double,double) ), this, SLOT( slotClicked(double,double) ) );
    connect( county, SIGNAL( mousemove(double,double) ), this, SLOT( slotMouseMove(double,double) ) );
}

void ddmStateModel::addBoundary( ddmBoundary* boundary, ddmCounty* county )
{
    county->addBoundary( boundary );
    this->m_boundaries.append( QVariant::fromValue( boundary ) );
}

void ddmStateModel::addVertex( const QSqlRecord& record, ddmBoundary* boundary )
{
    ddmPoint* vertex = boundary->addVertex( record );
    this->m_vertices.append( QVariant::fromValue( vertex ) );
}

QVariantList ddmStateModel::states() const
{
    return this->m_states;
}

int ddmStateModel::stateCount() const
{
    return this->states().count();
}

ddmState* ddmStateModel::state( int id ) const
{
    ddmState* state = NULL;
    QVariantList states = this->states();
    foreach ( QVariant obj, states )
    {
        ddmState* value = obj.value<ddmState*>();
        if ( value->id() == id )
        {
            state = value;
            break;
        }
    }

    return state;
}

ddmState* ddmStateModel::state( const QString& geographicName ) const
{
    ddmState* state = NULL;
    QVariantList states = this->states();
    foreach ( QVariant obj, states )
    {
        ddmState* value = obj.value<ddmState*>();
        if ( value->geographicName() == geographicName )
        {
            state = value;
            break;
        }
    }

    return state;
}

QVariantList ddmStateModel::counties() const
{
    return this->m_counties;
}

int ddmStateModel::countyCount() const
{
    return this->counties().count();
}

ddmCounty* ddmStateModel::county( int id ) const
{
    ddmCounty* county = NULL;
    QVariantList counties = this->counties();
    foreach ( QVariant obj, counties )
    {
        ddmCounty* value = obj.value<ddmCounty*>();
        if ( value->id() == id )
        {
            county = value;
            break;
        }
    }

    return county;
}

ddmCounty* ddmStateModel::county( const QString& geographicName ) const
{
    ddmCounty* county = NULL;
    QVariantList counties = this->counties();
    foreach ( QVariant obj, counties )
    {
        ddmCounty* value = obj.value<ddmCounty*>();
        if ( value->geographicName() == geographicName )
        {
            county = value;
            break;
        }
    }

    return county;
}

QVariantList ddmStateModel::boundaries() const
{
    return const_cast<ddmStateModel*>( this )->m_boundaries;
}

ddmBoundary* ddmStateModel::boundary( int id ) const
{
    ddmBoundary* boundary = NULL;
    QVariantList boundaries = this->boundaries();
    foreach ( QVariant obj, boundaries )
    {
        ddmBoundary* value = obj.value<ddmBoundary*>();
        if ( value->id() == id )
        {
            boundary = value;
            break;
        }
    }

    return boundary;
}

int ddmStateModel::boundaryCount() const
{
    return this->boundaries().size();
}

QVariantList ddmStateModel::vertices() const
{
    return this->m_vertices;
}

ddmPoint* ddmStateModel::vertex( int id ) const
{
    return this->vertices().takeAt( id ).value<ddmPoint*>();
}

int ddmStateModel::vertexCount() const
{
    return this->m_vertices.size();
}


void ddmStateModel::slotClicked( double x, double y )
{
    ddmCounty* county = qobject_cast<ddmCounty*>( sender() );
    int county_id = county ? county->id() : 0;
    qDebug() << "Clicked county" << county->geographicName() << "at [" << x << "," << y << "]";

    Q_EMIT clicked( county_id, x, y );
}

void ddmStateModel::slotMouseMove( double x, double y )
{
    ddmCounty* county = qobject_cast<ddmCounty*>( sender() );
    int county_id = county ? county->id() : 0;

    Q_EMIT mousemove( county_id, x, y );
}

/**
 * @brief ddmStateModel::~ddmStateModel
 */
ddmStateModel::~ddmStateModel()
{
}
