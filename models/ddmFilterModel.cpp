#include <QTime>
#include <QStringList>
#include <QCryptographicHash>
#include "base/ddmState.h"
#include "base/ddmCounty.h"
#include "base/ddmBoundary.h"
#include "models/ddmFilterModel.h"


/**
 * @brief ddmFilterModel::ddmFilterModel
 * @param parent
 */
ddmFilterModel::ddmFilterModel( QObject* parent ) : ddmStateModel( parent )
{
    this->setObjectName( "model" ); // делаем модель доступной в JavaScript
}

/**
 * Загружает данные из БД
 *
 * @author  Марунин А.В.
 * @since   2.3
 */
void ddmFilterModel::reloadData()
{
    // По умолчанию выбираем вообще все графства
    QString sqlQuery = QString( "SELECT * FROM cache_boundaries" );
    this->execQuery( sqlQuery );
}

/**
 * Извлекает набор графств по заданному SQL-запросу
 *
 * Для указанного SQL-запроса создается временная таблица, имя которой задается вторым параметром.
 * Если имя не указано, то берется имя модели
 *
 * @param   sql Запрос, который выбирает графства.
 *          В запросе обязательно должны быть указаны следующие поля
 *              - state_id      идентификатор штата
 *              - state_name    название штата
 *              - county_id     идентификатор графства
 *              - county_name   название графства
 *              - boundary_id   идентификатор контура графства
 *              - center_x      X-координата центра контура
 *              - center_y      Y-координата центра контура
 *          Настоятельно рекомендуется в запросе следующий порядок сортировки:
 *              - по state_id
 *              - по county_id
 *              - по остальным полям (если требуется)
 * @param   tempTable Имя временной таблицы, в которой будет храниться результат
 * @author  Марунин А.В.
 * @since   2.3
 */
void ddmFilterModel::execQuery( const QString& sqlQuery )
{

    ddmDatabase& db = this->database();
    QString sql;
    QSqlQueryModel* query = NULL;
    int rowCount;

    // Удаляем результаты предыдущего запроса
    this->clear();

    QTime timer;
    timer.start();

    QString tempTable = this->createTempTable( sqlQuery );
    // @DEBUG qDebug() << sqlQuery;

    // Резервируем память
    //this->reserveStatesMemory( tempTable );
    //this->reserveCountiesMemory( tempTable );
    //this->reserveBoundariesMemory( tempTable );

    // Выполняем новый запрос
    qDebug() << "Fetching counties...";
    sql = QString( "SELECT * FROM %1" ).arg( tempTable );
    query = db.select( sql );
    Q_ASSERT( !db.hasErrors() );

    ddmState* currentState = NULL;
    ddmCounty* currentCounty = NULL;
    ddmBoundary* currentBoundary = NULL;
    QStringList boundaryIds;
    rowCount = query->rowCount();
    for ( int i = 0; i < rowCount; i++ )
    {
        // @DEBUG if ( i > 1000 && i % 1000 == 0 ) qDebug() << i << "/" << rowCount << "rows...";

        QSqlRecord record = query->record( i );
        int state_id = record.value( "state_id" ).toInt();
        if ( !state_id ) continue;

        if ( !currentState || state_id != currentState->id() )
        {
            ddmState* state = this->findState( state_id );
            if ( !state )
            {
                state = new ddmState( record );
                this->m_totalStates.insert( state_id, state );
            }
            this->addState( state );
            currentState = state;
        }

        int county_id = record.value( "county_id" ).toInt();
        Q_ASSERT( county_id > 0 );
        if ( !currentCounty || county_id != currentCounty->id() )
        {
            ddmCounty* county = this->findCounty( county_id );
            if ( !county )
            {
                county = new ddmCounty( record );
                this->m_totalCounties.insert( county_id, county );
            }
            this->addCounty( county, currentState );
            currentCounty = county;
        }

        int boundary_id = record.value( "boundary_id" ).toInt();
        Q_ASSERT( boundary_id > 0 );
        if ( !currentBoundary || boundary_id != currentBoundary->id() )
        {
            ddmBoundary* boundary = this->findBoundary( boundary_id );
            if ( !boundary )
            {
                boundary = new ddmBoundary( record );
                this->m_totalBoundaries.insert( boundary_id, boundary );
            }
            this->addBoundary( boundary, currentCounty );
            boundaryIds.append( record.value( "boundary_id" ).toString() );
            currentBoundary = boundary;
        }
    }

    // @DEBUG
    /*
    qDebug() << "States:" << this->stateCount();
    qDebug() << "Counties:" << this->countyCount();
    qDebug() << "Boundaries:" << this->boundaryCount();
    qDebug() << "Elapsed:" << timer.elapsed() / 1000.0 << "sec";
    */

    // Извлекаем информацию о вершинах
    // Берем только те контура, которые относятся к найденным графствам
    qDebug() << "Fetching vertices...";

    sql = QString(
        "SELECT tt.boundary_id, bp.x AS x, bp.y AS y, bp.point_id AS point_id\n"
        "  FROM %1 AS tt\n"
        "  LEFT JOIN cache_boundary_points AS bp ON tt.boundary_id = bp.boundary_id"
        "  ORDER BY tt.boundary_id"
    ).arg( tempTable );
    //qDebug() << sql;
    query = db.select( sql );
    Q_ASSERT( !db.hasErrors() );

    rowCount = query->rowCount();
    this->m_vertices.reserve( rowCount );
    qDebug() << rowCount << "vertices";
    currentBoundary = NULL;
    QTime rt;
    rt.start();
    for ( int i = 0; i < rowCount; i++ )
    {
        // @DEBUG
        /*
        if ( i > 1000 && i % 1000 == 0 )
        {
            int elapsed = rt.elapsed();
            qDebug() << i << "/" << rowCount << "rows" << "(" << 0.001 * elapsed << "sec" << ")";
            rt.restart();
        }
        */
        QSqlRecord record = query->record( i );
        int boundary_id = record.value( "boundary_id" ).toInt();
        Q_ASSERT( boundary_id > 0 );
        if ( !currentBoundary || boundary_id != currentBoundary->id() )
        {
            currentBoundary = this->boundary( boundary_id );
        }
        Q_ASSERT( currentBoundary != NULL );
        this->addVertex( record, currentBoundary );
    }

    // @DEBUG
    // qDebug() << "Vertices:" << this->vertexCount() << ", elapsed:" << timer.elapsed() / 1000.0 << "sec";

}

ddmState* ddmFilterModel::findState( int state_id ) const
{
    QMap<int, ddmState*>::const_iterator It = this->m_totalStates.find( state_id );
    return ( It != this->m_totalStates.end() ? It.value() : NULL );
}

ddmState* ddmFilterModel::findState( const QString& state_name ) const
{
    ddmState* state = NULL;
    foreach ( ddmState* item, this->m_totalStates )
    {
        if ( item->geographicName() == state_name )
        {
            state = item;
        }
    }

    return state;
}

ddmCounty* ddmFilterModel::findCounty( int county_id ) const
{
    QMap<int, ddmCounty*>::const_iterator It = this->m_totalCounties.find( county_id );
    return ( It != this->m_totalCounties.end() ? It.value() : NULL );
}

ddmCounty* ddmFilterModel::findCounty( const QString& county_name ) const
{
    ddmCounty* county = NULL;
    foreach ( ddmCounty* item, this->m_totalCounties )
    {
        if ( item->geographicName() == county_name )
        {
            county = item;
        }
    }

    return county;
}

ddmBoundary* ddmFilterModel::findBoundary( int boundary_id ) const
{
    QMap<int, ddmBoundary*>::const_iterator It = this->m_totalBoundaries.find( boundary_id );
    return ( It != this->m_totalBoundaries.end() ? It.value() : NULL );
}

ddmStateMap& ddmFilterModel::totalStates() const
{
    return const_cast<ddmFilterModel*>( this )->m_totalStates;
}

ddmCountyMap& ddmFilterModel::totalCounties() const
{
    return const_cast<ddmFilterModel*>( this )->m_totalCounties;
}

ddmBoundaryMap& ddmFilterModel::totalBoundaries() const
{
    return const_cast<ddmFilterModel*>( this )->m_totalBoundaries;
}

/**
 * Резервирует память для штатов
 *
 * Определяет, сколько памяти нужно выделить для хранения списка штатов
 *
 * @param table_name Имя таблицы
 */
void ddmFilterModel::reserveStatesMemory( const QString& table_name )
{
    ddmDatabase& db = this->database();
    QString sql = QString( "SELECT COUNT(state_id) FROM %1 GROUP BY state_id" ).arg( table_name );
    QSqlQueryModel* query = db.select( sql );
    Q_ASSERT( !db.hasErrors() );
    int n = query->data( query->index( 0, 0 ) ).toInt();
    if ( n > 0 )
    {
        this->m_states.reserve( n );
    }
    qDebug() << n << "states";
}

/**
 * Резервирует память для графств
 *
 * Определяет, сколько памяти нужно выделить для хранения списка графств
 *
 * @param table_name Имя таблицы
 */
void ddmFilterModel::reserveCountiesMemory( const QString& table_name )
{
    ddmDatabase& db = this->database();
    QString sql = QString( "SELECT COUNT(county_id) FROM %1 GROUP BY county_id" ).arg( table_name );
    QSqlQueryModel* query = db.select( sql );
    Q_ASSERT( !db.hasErrors() );
    int n = query->data( query->index( 0, 0 ) ).toInt();
    if ( n > 0 )
    {
        this->m_counties.reserve( n );
    }
    qDebug() << n << "counties";
}

/**
 * Резервирует память для контуров графств
 *
 * Определяет, сколько памяти нужно выделить для хранения списка контуров графств
 *
 * @param table_name Имя таблицы
 */
void ddmFilterModel::reserveBoundariesMemory( const QString& table_name )
{
    ddmDatabase& db = this->database();
    QString sql = QString( "SELECT COUNT(boundary_id) FROM %1 GROUP BY boundary_id" ).arg( table_name );
    QSqlQueryModel* query = db.select( sql );
    Q_ASSERT( !db.hasErrors() );
    int n = query->data( query->index( 0, 0 ) ).toInt();
    if ( n > 0 )
    {
        this->m_boundaries.reserve( n );
    }
    qDebug() << n << "boundaries";
}

/**
 * Создает временную таблицу
 *
 * Создает временную таблицу в БД согласно указанному запросу.
 * По окончании работы приложения таблица будет уничтожена.
 *
 * @param   sqlQuery SQL-запрос, по которому создается таблица
 * @param   force Флаг, указывающий принудительно создать таблицу
 *          в случае повторного вызова метода (по умолчанию таблица создается лишь при первом вызове)
 * @return  Название таблицы.
 */
QString ddmFilterModel::createTempTable( const QString& sqlQuery, bool force )
{
    ddmDatabase& db = this->database();
    QString sql;

    QString queryHash = QCryptographicHash::hash( sqlQuery.toStdString().c_str(), QCryptographicHash::Md5 ).toHex();
    QString tableName = QString( "temp_%1" ).arg( queryHash );

    if ( force )
    {
        sql = QString( "DROP TABLE IF EXISTS %1" ).arg( tableName );
        db.exec( sql );
        Q_ASSERT( !db.hasErrors() );
    }

    sql = QString( "CREATE TEMP TABLE IF NOT EXISTS %1 AS %2" ).arg( tableName ).arg( sqlQuery );
    db.exec( sql );
    Q_ASSERT( !db.hasErrors() );

    return tableName;
}

/**
 * @brief ddmFilterModel::~ddmFilterModel
 */
ddmFilterModel::~ddmFilterModel()
{
}

