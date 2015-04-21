#include "base/ddmCounty.h"
#include "models/ddmMaxInOutNeighborFilterModel.h"


/**
 * Конструктор класса
 *
 * @param   parent Родитель (владелец) модели
 * @author  Марунин А.В.
 * @since   2.10
 */
ddmMaxInOutNeighborFilterModel::ddmMaxInOutNeighborFilterModel( QObject* parent ) : ddmFilterModel( parent ),
    m_neighborType( NT_FIRST_NEIGHBOR ), m_sameState( true )
{
    this->m_neighborFilter = new ddmFilterModel( this );
}

/**
 * Загружает данные из БД
 *
 * @author  Марунин А.В.
 * @since   2.10
 */
void ddmMaxInOutNeighborFilterModel::reloadData()
{
    ddmDatabase& db = this->database();
    QString sqlQuery;
    QSqlQueryModel* query;
    int rowCount;

    this->prepareTempTable();

    if ( this->neighborType() == NT_FIRST_NEIGHBOR )
    {
        sqlQuery = QString(
            "SELECT n.county_a AS county_id, n.county_b AS neighbor_id"
            "  FROM ddm_county_neighbors AS n"
            "  LEFT JOIN %1 AS t ON t.county_id = n.county_a"
            "  WHERE t.county_in_id = n.county_b AND t.county_out_id = n.county_b"
            ).arg( this->m_tableMaxInOutCountries );
        QString tmpTable1 = this->createTempTable( sqlQuery );

        sqlQuery = QString(
            "SELECT c.* FROM %1 AS t"
            "  LEFT JOIN cache_boundaries AS c ON c.county_id = t.neighbor_id"
            ).arg( tmpTable1 );
        query = db.select( sqlQuery );
        Q_ASSERT( !db.hasErrors() );
        rowCount = query->rowCount();
        qDebug() << "Rows:" << rowCount;

        sqlQuery = QString(
            "SELECT c.* FROM %1 AS t"
            "  LEFT JOIN cache_boundaries AS c ON c.county_id = t.county_id"
            ).arg( tmpTable1 );
        this->execQuery( sqlQuery );
        qDebug() << "Found counties:" << this->countyCount();

        sqlQuery = QString(
            "SELECT c.* FROM %1 AS t"
            "  LEFT JOIN cache_boundaries AS c ON c.county_id = t.neighbor_id"
            ).arg( tmpTable1 );
        this->m_neighborFilter->execQuery( sqlQuery );
        this->m_neighbors = this->m_neighborFilter->counties();
        qDebug() << "Found neighbors:" << this->m_neighbors.size();
    }
    else if ( this->neighborType() == NT_SECOND_NEIGHBOR )
    {

    }
    else if ( this->neighborType() == NT_FAR_NEIGHBOR )
    {

    }
    else
    {
        sqlQuery = QString(
            "SELECT c.* FROM cache_boundaries AS c"
            "  LEFT JOIN %1 AS t ON t.county_id = c.county_id"
            "  WHERE t.county_out_id = t.county_in_id"
            ).arg( this->m_tableMaxInOutCountries );
        this->execQuery( sqlQuery );
    }
}

/**
 * Подготавливает промежуточные временные таблицы
 *
 * @author  Марунин А.В.
 * @since   2.10
 */
void ddmMaxInOutNeighborFilterModel::prepareTempTable()
{
    if ( this->m_tableMaxInOutCountries.isEmpty() )
    {
        QString sqlQuery;

        sqlQuery = QString( "SELECT county_a, county_b, in_estimate FROM ddm_mirgations ORDER BY county_a, in_estimate" );
        QString tableMaxInAll = this->createTempTable( sqlQuery );

        sqlQuery = QString( "SELECT * FROM %1 GROUP BY county_a" ).arg( tableMaxInAll );
        QString tableMaxInCountries = this->createTempTable( sqlQuery );

        sqlQuery = QString( "SELECT county_a, county_b, out_estimate FROM ddm_mirgations ORDER BY county_a, out_estimate" );
        QString tableMaxOutAll = this->createTempTable( sqlQuery );

        sqlQuery = QString( "SELECT * FROM %1 GROUP BY county_a" ).arg( tableMaxOutAll );
        QString tableMaxOutCountries = this->createTempTable( sqlQuery );

        sqlQuery = QString(
                    "SELECT tin.county_a  AS county_id, "
                    "       tin.county_b  AS county_in_id,  tin.in_estimate AS in_estimate, "
                    "       tout.county_b AS county_out_id, tout.out_estimate AS out_estimate "
                    "  FROM ddm_counties AS c"
                    "  INNER JOIN ddm_residences AS r ON c.id  = r.county_id"
                    "  LEFT JOIN %1 AS tin  ON tin.county_a  = r.county_id"
                    "  LEFT JOIN %2 AS tout ON tout.county_a = r.county_id"
                )
                .arg( tableMaxInCountries )
                .arg( tableMaxOutCountries );
        this->m_tableMaxInOutCountries = this->createTempTable( sqlQuery );
    }
}

/**
 * @brief ddmMaxInOutNeighborFilterModel::neighborType
 * @return  Целочисленное значение - тип соседства
 * @author  Марунин А.В.
 * @since   2.10
 * @see     ddmNeighborType
 */
ddmNeighborType ddmMaxInOutNeighborFilterModel::neighborType() const
{
    return this->m_neighborType;
}

/**
 * Задает соседство между найденными графствами
 *
 * @param   type Тип соседства графств
 * @author  Марунин А.В.
 * @since   2.10
 * @see     ddmNeighborType
 */
void ddmMaxInOutNeighborFilterModel::setNeighborType( ddmNeighborType type )
{
    if ( this->neighborType() != type )
    {
        this->m_neighborType = type;
        Q_EMIT changed();
    }
}

/**
 * Возвращает флаг принадлежности к одному штату
 *
 * @return  true, если найденные графства должны принадлежать к одному штату
 * @author  Марунин А.В.
 * @since   2.10
 */
bool ddmMaxInOutNeighborFilterModel::sameState() const
{
    return this->m_sameState;
}

/**
 * Задает флаг принадлежности графств к одному штату
 *
 * @param   flag Флаг, указывающий на принадлежность к одному штату.
 * @author  Марунин А.В.
 * @since   2.10
 */
void ddmMaxInOutNeighborFilterModel::setSameState( bool flag )
{
    if ( this->sameState() != flag )
    {
        this->m_sameState = flag;
        Q_EMIT changed();
    }
}

/**
 * Задает настройки соседства
 *
 * @param   neighborType Тип соседства
 * @param   sameState Должны ли графства быть из одного штата
 * @author  Марунин А.В.
 * @since   2.10
 */
void ddmMaxInOutNeighborFilterModel::setNeighborOptions( ddmNeighborType neighborType, bool sameState )
{
    if ( this->neighborType() != neighborType || this->sameState() != sameState )
    {
        this->m_neighborType = neighborType;
        this->m_sameState = sameState;
        Q_EMIT changed();
    }
}

/**
 * Возвращает список имен графств
 *
 * Список содержит и графства, и соседей.
 *
 * @return  Объект типа QStringList
 * @author  Марунин А.В.
 * @since   2.10
 */
QStringList ddmMaxInOutNeighborFilterModel::countyNames() const
{
    QStringList names;
    foreach ( QVariant obj, this->m_counties )
    {
        ddmCounty* county = obj.value<ddmCounty*>();
        names.append( county->geographicName() );
    }
    foreach ( QVariant obj, this->m_neighbors )
    {
        ddmCounty* county = obj.value<ddmCounty*>();
        names.append( county->geographicName() );
    }

    return names;
}

/**
 * Возвращает список соседей
 *
 * @return  Объект типа QVariantList
 * @author  Марунин А.В.
 * @since   2.10
 */
QVariantList ddmMaxInOutNeighborFilterModel::neighbors() const
{
    return this->m_neighbors;
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.10
 */
ddmMaxInOutNeighborFilterModel::~ddmMaxInOutNeighborFilterModel()
{
}

