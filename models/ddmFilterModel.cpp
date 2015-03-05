#include <QtDebug>
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

    QString queryHash = QCryptographicHash::hash( sqlQuery.toStdString().c_str(), QCryptographicHash::Md5 ).toHex();
    QString tempTable = QString( "temp_%1" ).arg( queryHash );
    sql = QString( "CREATE TEMP TABLE IF NOT EXISTS %1 AS %2" ).arg( tempTable ).arg( sqlQuery );
    db.exec( sql );
    Q_ASSERT( !db.hasErrors() );

    qDebug() << sqlQuery;

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
        if ( i > 1000 && i % 1000 == 0 ) qDebug() << i << "/" << rowCount << "rows...";

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

    qDebug() << "States:" << this->stateCount();
    qDebug() << "Counties:" << this->countyCount();
    qDebug() << "Boundaries:" << this->boundaryCount();
    qDebug() << "Elapsed:" << timer.elapsed() / 1000.0 << "sec";


    // Извлекаем информацию о вершинах
    // Берем только те контура, которые относятся к найденным графствам
    qDebug() << "Fetching vertices...";

    sql = QString(
        "SELECT tt.boundary_id, bp.x AS x, bp.y AS y\n"
        "  FROM %1 AS tt\n"
        "  LEFT JOIN cache_boundary_points AS bp ON tt.boundary_id = bp.boundary_id"
        "  ORDER BY tt.boundary_id"
    ).arg( tempTable );
    //qDebug() << sql;
    query = db.select( sql );
    Q_ASSERT( !db.hasErrors() );

    rowCount = query->rowCount();
    currentBoundary = NULL;
    QTime rt;
    rt.start();
    for ( int i = 0; i < rowCount; i++ )
    {
        if ( i > 1000 && i % 1000 == 0 )
        {
            int elapsed = rt.elapsed();
            qDebug() << i << "/" << rowCount << "rows" << "(" << 0.001 * elapsed << "sec" << ")";
            rt.restart();
        }
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

    qDebug() << "Vertices:" << this->vertexCount() << ", elapsed:" << timer.elapsed() / 1000.0 << "sec";

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
 * @brief ddmFilterModel::~ddmFilterModel
 */
ddmFilterModel::~ddmFilterModel()
{
}

