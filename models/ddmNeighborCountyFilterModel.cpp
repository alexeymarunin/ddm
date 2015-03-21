#include <QSqlRecord>

#include "models/ddmNeighborCountyFilterModel.h"

/**
 * Конструктор класса
 *
 * @param   parent Владелец модели
 * @author  Марунин А.В.
 * @since   2.8
 */
ddmNeighborCountyFilterModel::ddmNeighborCountyFilterModel( QObject* parent ) : ddmBaseCountyFilterModel( parent )
{
}

/**
 * Загружает данные из БД
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmNeighborCountyFilterModel::reloadData()
{
    if ( this->currentState() )
    {
        this->loadState( this->currentState()->id() );
        this->updateCountyNames();
        if ( this->currentCounty() )
        {
            this->updateNeighbors();

            QStringList neighbors;
            neighbors.append( QString( "%1" ).arg( this->currentCounty()->id() ) );
            foreach ( int county_id, this->m_neighbors )
            {
                neighbors.append( QString( "%1" ).arg( county_id ) );
            }

            // Выбираем соседние графства текущего штата и само текущее графство
            QString sqlQuery = QString( "SELECT * FROM cache_boundaries WHERE county_id IN (%1)" ).arg( neighbors.join( ',' ) );
            //qDebug() << sqlQuery;
            this->execQuery( sqlQuery );
        }
    }
}

/**
 * Обновляет список соседей у текущего графства
 *
 * Находит все id соседей первого порядка
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmNeighborCountyFilterModel::updateNeighbors()
{
    this->m_neighbors.clear();
    if ( this->currentCounty() )
    {
        ddmDatabase& db = this->database();
        QString sql;
        QSqlQueryModel* query;
        int i, rowCount;

        sql = QString( "SELECT county_b FROM ddm_county_neighbors WHERE county_a = %1" ).arg( this->currentCounty()->id() );
        query = db.select( sql );
        Q_ASSERT( db.hasErrors() == false );
        rowCount = query->rowCount();
        for ( i = 0; i < rowCount; i++ )
        {
            int county_id = query->data( query->index( i, 0 ) ).toInt();
            this->m_neighbors.insert( county_id );
            // qDebug() << county_id;
        }

        sql = QString( "SELECT county_a FROM ddm_county_neighbors WHERE county_b = %1" ).arg( this->currentCounty()->id() );
        query = db.select( sql );
        Q_ASSERT( db.hasErrors() == false );
        rowCount = query->rowCount();
        for ( i = 0; i < rowCount; i++ )
        {
            int county_id = query->data( query->index( i, 0 ) ).toInt();
            this->m_neighbors.insert( county_id );
            // qDebug() << county_id;
        }
    }
}

/**
 * Возвращает список id соседей
 *
 * @return  Объект типа QSet<int>
 * @author  Марунин А.В.
 * @since   2.8
 */
QSet<int> ddmNeighborCountyFilterModel::neighbors() const
{
    return this->m_neighbors;
}

void ddmNeighborCountyFilterModel::slotClicked( double x, double y )
{
    Q_UNUSED( x );
    Q_UNUSED( y );
    ddmCounty* county = qobject_cast<ddmCounty*>( sender() );
    //this->setCurrentCounty( county );
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
ddmNeighborCountyFilterModel::~ddmNeighborCountyFilterModel()
{
}
