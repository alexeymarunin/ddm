#ifndef DDM_DATABASE_H
#define DDM_DATABASE_H

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QCache>

#include "base/ddmObject.h"


/**
 * Класс ddmDatabase содержит методы для извлечения данных из БД,
 * а также выполнения некоторых других операций с БД
 * Напрямую создать объект данного типа нельзя (это singletone),
 * для доступа к его методам используется instance()
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
class ddmDatabase : public ddmObject
{
    Q_OBJECT

public:

    static ddmDatabase& instance();

    QSqlQueryModel* select( const QString& sql, bool force = false );
    QSqlQuery exec( const QString& sql );

    QSqlError lastError() const;
    bool hasErrors() const;

    QSqlDatabase& db();

    void close();

    virtual ~ddmDatabase();

private:

    typedef QCache<QString, QSqlQueryModel> ddmDatabaseCache;

    static ddmDatabase* m_instance;
    QSqlDatabase        m_db;
    QSqlError           m_lastError;
    ddmDatabaseCache    m_cache;

    ddmDatabase( QObject* parent );
};

#endif // DDM_DATABASE_H
