#include <QtGlobal>
#include <QStringList>
#include <QDir>
#include <QTime>
#include <QtDebug>

#include "ddmApplication.h"
#include "base/ddmDatabase.h"

#define DDM_DATABASE_NAME "ddm.sqlite"

ddmDatabase* ddmDatabase::m_instance = NULL;

/**
 * Констуктор по умолчанию
 *
 * @param   parent Родитель (владелец) экземпляра класса
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmDatabase::ddmDatabase( QObject* parent ) : ddmObject( parent )
{
    this->m_db = QSqlDatabase::addDatabase( "QSQLITE" );

    // Формируем список каталогов, в которых может лежать БД
    QString currentDir = QDir::current().absolutePath();
    QStringList dirs;
    dirs.append( currentDir );
    dirs.append( QObject::tr( "%1/../.." ).arg( currentDir ) );
    dirs.append( QObject::tr( "%1/.." ).arg( currentDir ) );

    QSqlDatabase& db = this->db();
    foreach ( QString dir, dirs )
    {
        QString path = QObject::tr( "%1/%2" ).arg( dir ).arg( DDM_DATABASE_NAME );
        if ( QFile::exists( path ) )
        {
            path = QDir( path ).absolutePath();
            // Пытаемся открыть БД
            qDebug() << "Found database " << path;
            db.setDatabaseName( path );
            bool dbOpened = db.open();
            Q_UNUSED( dbOpened );
            Q_ASSERT_X( dbOpened, "ddmDatabase::open", qPrintable( db.lastError().text() ) );
            break;
        }
    }
}

/**
 * Выполняет SELECT-запрос к БД
 *
 * @param   sql Строка SQL-кода
 * @param   force Флаг, указывающий производить ли принудительную очистку кэша
 * @return  Указатель на объект типа QSqlQueryModel
 * @author  Марунин А.В.
 * @since   2.0
 */
QSqlQueryModel* ddmDatabase::select( const QString& sql, bool force )
{
    // qDebug() << sql;
    QSqlQueryModel* queryModel;

    QTime timer;
    timer.start();

    ddmDatabaseCache& cache = this->m_cache;
    if ( force || !cache.contains( sql ) )
    {
        queryModel = new QSqlQueryModel( this );
        queryModel->setQuery( sql, this->db() );
        while ( queryModel->canFetchMore() )
        {
            queryModel->fetchMore();
        }
        cache.insert( sql, queryModel );
    }
    else
    {
        queryModel = cache[sql];
    }

    this->m_lastError = queryModel->lastError();

    if ( this->hasErrors() )
    {
        qDebug() << this->lastError().text();
    }

    return queryModel;
}

/**
 * Выполняет указанный SQL-код
 *
 * @param   sql Строка SQL-кода
 * @return  Объект типа QSqlQuery
 * @author  Марунин А.В.
 * @since   2.0
 */
QSqlQuery ddmDatabase::exec( const QString& sql )
{
    // qDebug() << sql;
    QSqlQuery query = this->db().exec( sql );
    this->m_lastError = query.lastError();
    if ( this->hasErrors() )
    {
        qDebug() << this->lastError().text();
    }
    return query;
}

/**
 * Возвращает ссылку на свой экземпляр
 *
 * @return  Объект типа ddmDatabase
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmDatabase& ddmDatabase::instance()
{
    if ( !ddmDatabase::m_instance ) {
        ddmDatabase::m_instance = new ddmDatabase( ddmApp );
    }

    return *ddmDatabase::m_instance;
}

/**
 * Возвращает ссылку на Qt-"движок" БД
 *
 * @return  Объект типа QSqlDatabase
 * @author  Марунин А.В.
 * @since   2.0
 */
QSqlDatabase& ddmDatabase::db()
{
    return ddmDatabase::m_db;
}

/**
 * Возращает информацию об ошибках при последнем SQL-запросе
 * @return  Объект типа QSqlError
 * @see     hasErrors()
 * @author  Марунин А.В.
 * @since   2.0
 */
QSqlError ddmDatabase::lastError() const
{
    return this->m_lastError;
}

/**
 * Определяет, имеются ли ошибки при выполнении последнего SQL-запроса к БД
 *
 * @return  true, если были ошибки
 * @see     lastError()
 * @author  Марунин А.В.
 * @since   2.0
 */
bool ddmDatabase::hasErrors() const
{
    return this->lastError().isValid();
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmDatabase::~ddmDatabase()
{
}
