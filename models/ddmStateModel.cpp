#include <QTime>
#include <QMap>

#include "models/ddmStateModel.h"
#include "base/ddmState.h"
#include "base/ddmCounty.h"
#include "base/ddmBoundary.h"

/**
 * Конструктор класса
 *
 * @param   parent Родитель (владелец) модели
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmStateModel::ddmStateModel( QObject* parent ) : ddmModel( parent )
{
    // Кэшируем данные
    // Операция хоть и долгая, но будет выполнена лишь 1 раз
    this->prepareCache();
    //this->prepareCache( true );
}

/**
 * Загружает данные из БД
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmStateModel::reloadData()
{
}

/**
 * Кэширует данные из БД
 *
 * Создает временные таблицы на основе "тяжелых" запросов,
 * чтобы при последующих вызовах сократить время обработки
 *
 * @author  Марунин А.В.
 * @since   2.3
 */
void ddmStateModel::prepareCache( bool force )
{
    ddmDatabase& db = this->database();
    QString sql;

    QTime timer;
    timer.start();
    int elapsed = 0;

    if ( force )
    {
        qDebug() << "Dropping cache...";

        sql = "DROP TABLE IF EXISTS cache_boundary_points";
        db.exec( sql );
        Q_ASSERT( !db.hasErrors() );

        sql = "DROP TABLE IF EXISTS cache_boundaries";
        db.exec( sql );
        Q_ASSERT( !db.hasErrors() );

        elapsed = timer.elapsed() - elapsed;
        qDebug() << "Elapsed:" << 0.001 * elapsed << "sec";
    }


    qDebug() << "Preparing vertices cache...";
    sql =
        "CREATE TABLE IF NOT EXISTS cache_boundary_points AS\n"
        "  SELECT bp.boundary_id, p.x, p.y, bp.point_id \n"
        "    FROM ddm_boundary_points AS bp\n"
        "    LEFT JOIN ddm_points AS p ON p.id = bp.point_id\n";
    db.exec( sql );
    Q_ASSERT( !db.hasErrors() );
    elapsed = timer.elapsed() - elapsed;
    qDebug() << "Elapsed:" << 0.001 * elapsed << "sec";


    qDebug() << "Preparing migrations cache...";
    sql =
        "CREATE TABLE IF NOT EXISTS cache_deltas AS\n"
        "  SELECT "
        "      m.county_b AS county_id, c.geographic_name AS county_name,\n"
        "      SUM( m.in_estimate ) AS county_in_sum, SUM( m.out_estimate ) AS county_out_sum,\n"
        "      SUM( m.in_estimate ) - SUM( m.out_estimate ) AS county_delta \n"
        "    FROM ddm_mirgations AS m\n"
        "    LEFT JOIN ddm_counties AS c ON c.id = m.county_b\n"
        "    GROUP BY m.county_b\n"
        "    ORDER BY m.county_b";

    db.exec( sql );
    Q_ASSERT( !db.hasErrors() );
    elapsed = timer.elapsed() - elapsed;
    qDebug() << "Elapsed:" << 0.001 * elapsed;


    qDebug() << "Preparing boundaries cache...";
    sql =
        "CREATE TABLE IF NOT EXISTS cache_boundaries AS\n"
        "  SELECT "
        "      s.id AS state_id, s.name AS state_name,\n"
        "      c.id AS county_id, c.geographic_name AS county_name,\n"
        "      r.popul_est AS county_population, \n"
        "      b.id AS boundary_id, p.x AS center_x, p.y center_y, b.square AS boundary_square,\n"
        "      f.f_out_sum AS county_f_out_sum, f.f_out_mid AS county_f_out_mid,\n"
        "      f.f_in_sum AS county_f_in_sum, f.f_in_mid AS county_f_in_mid,\n"
        "      f.f_mid AS county_f_mid,\n"
        "      d.county_in_sum AS county_in_sum, d.county_out_sum AS county_out_sum, d.county_delta AS county_delta\n"
        "    FROM ddm_county_boundaries AS cb\n"
        "    LEFT JOIN ddm_boundaries AS b ON b.id = cb.boundary_id\n"
        "    LEFT JOIN ddm_points AS p ON p.id = b.center_id\n"
        "    LEFT JOIN ddm_residences  AS r ON r.county_id = cb.county_id\n"
        "    INNER JOIN ddm_frictions AS f ON f.county_id = cb.county_id\n"
        "    INNER JOIN cache_deltas AS d ON d.county_id = cb.county_id\n"
        "    LEFT JOIN ddm_counties AS c ON c.id = f.county_id\n"
        "    LEFT JOIN ddm_states AS s ON s.id = c.state_id\n"
        "    WHERE b.outer = 1\n"
        "    ORDER BY s.id, c.id, b.square DESC";

    db.exec( sql );
    Q_ASSERT( !db.hasErrors() );
    elapsed = timer.elapsed() - elapsed;
    qDebug() << "Elapsed:" << 0.001 * elapsed;


    // Создаем индексы в таблицах
    sql = "CREATE INDEX IF NOT EXISTS cache_state_id_idx ON cache_boundaries ( state_id )";
    db.exec( sql );
    Q_ASSERT( !db.hasErrors() );

    sql = "CREATE INDEX IF NOT EXISTS cache_county_id_idx ON cache_boundaries ( county_id )";
    db.exec( sql );
    Q_ASSERT( !db.hasErrors() );

    sql = "CREATE INDEX IF NOT EXISTS cache_boundary_id_idx ON cache_boundaries ( boundary_id )";
    db.exec( sql );
    Q_ASSERT( !db.hasErrors() );

    sql = "CREATE INDEX IF NOT EXISTS cache_delta_idx ON cache_boundaries ( county_delta\n )";
    db.exec( sql );
    Q_ASSERT( !db.hasErrors() );

    sql = "CREATE INDEX IF NOT EXISTS cache_bp_id_idx ON cache_boundary_points ( boundary_id )";
    db.exec( sql );
    Q_ASSERT( !db.hasErrors() );

    elapsed = timer.elapsed() - elapsed;
    qDebug() << "Elapsed:" << 0.001 * elapsed << "sec";

    qDebug() << "====================================================";
    qDebug() << "Total time:" << 0.001 * timer.elapsed() << "sec\n";
}


/**
 * Добавляет новый штат в модель
 *
 * Проверка на дубликаты не производится!
 *
 * @param   state Добавляемый штат
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmStateModel::addState( ddmState* state )
{
    state->setParent( this );
    this->m_states.append( QVariant::fromValue( state ) );
}

/**
 * Добавляет графство в заданный штат
 *
 * Проверка на дубликаты не производится!
 *
 * @param   county Добавляемое графство
 * @param   state Штат, в который добавляется графство
 */
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
    return this->m_states.count();
}

ddmState* ddmStateModel::state( int id ) const
{
    ddmState* state = NULL;
    foreach ( QVariant obj, this->m_states )
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
    foreach ( QVariant obj, this->m_states )
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
    return this->m_counties.count();
}

ddmCounty* ddmStateModel::county( int id ) const
{
    ddmCounty* county = NULL;
    foreach ( QVariant obj, this->m_counties )
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
    foreach ( QVariant obj, this->m_counties )
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
    foreach ( QVariant obj, this->m_boundaries )
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
    return this->m_boundaries.size();
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

/**
 * @brief ddmStateModel::clear
 * @author  Марунин А.В.
 * @since   2.3
 */
void ddmStateModel::clear()
{
    this->m_states.clear();
    this->m_counties.clear();
    this->m_boundaries.clear();
    this->m_vertices.clear();
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
