#ifndef DDM_STATE_H
#define DDM_STATE_H

#include "base/ddmMapObject.h"

class ddmCounty;

/**
 * Класс ddmState описывает штат
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
class ddmState : public ddmMapObject
{
    Q_OBJECT

public:

    ddmState( int id = 0, const QString& geographicName = "", QObject* parent = 0 );
    ddmState( const QSqlRecord& record, QObject* parent = 0 );

    Q_PROPERTY( QString name READ geographicName )
    Q_PROPERTY( QVariantList counties READ counties )

    void addCounty( ddmCounty* county );

    QString geographicName() const;

    QVariantList counties() const;
    int countyCount() const;
    ddmCounty* county( int id ) const;
    ddmCounty* county( const QString& geographicName ) const;

    QStringList countyNames() const;

    virtual ~ddmState();

protected:

    QString m_geographicName;

    virtual void create( const QSqlRecord& record );
    void create( int id, const QString& geographicName );
};

#endif // DDM_STATE_H
