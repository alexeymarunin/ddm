#ifndef DDM_STATE_MODEL_H
#define DDM_STATE_MODEL_H

#include <QSignalMapper>
#include "models/ddmModel.h"

class ddmState;
class ddmCounty;
class ddmBoundary;
class ddmPoint;

/**
 * Класс ddmStateModel описывает модель данных, которая хранит информацию обо всех штатах и их графствах
 *
 * Данная модель является ключевой - она используется в качестве базовой для остальных моделей приложения
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
class ddmStateModel : public ddmModel
{
    Q_OBJECT

public:

    ddmStateModel( QObject* parent = 0 );

    Q_PROPERTY( QVariantList states READ states )
    Q_PROPERTY( int stateCount READ stateCount )
    Q_PROPERTY( QVariantList counties READ counties )
    Q_PROPERTY( int countyCount READ countyCount )
    Q_PROPERTY( QVariantList boundaries READ boundaries )
    Q_PROPERTY( int boundaryCount READ boundaryCount )
    Q_PROPERTY( QVariantList vertices READ vertices )
    Q_PROPERTY( int vertexCount READ vertexCount )

    ddmState* state( int id ) const;
    ddmState* state( const QString& geographicName ) const;
    QVariantList states() const;
    int stateCount() const;

    ddmCounty* county( int id ) const;
    ddmCounty* county( const QString& geographicName ) const;
    QVariantList counties() const;
    int countyCount() const;

    QVariantList boundaries() const;
    ddmBoundary* boundary( int id ) const;
    int boundaryCount() const;

    QVariantList vertices() const;
    ddmPoint* vertex( int id ) const;
    int vertexCount() const;

    virtual void reloadData();
    void clear();

    virtual ~ddmStateModel();

Q_SIGNALS:

    void clicked( int county_id, double x, double y );
    void mousemove( int county_id, double x, double y );

protected Q_SLOTS:

    void slotClicked( double x, double y );
    void slotMouseMove( double x, double y );

protected:

    QVariantList    m_states;
    QVariantList    m_counties;
    QVariantList    m_boundaries;
    QVariantList    m_vertices;

    void addState( ddmState* state );
    void addCounty( ddmCounty* county, ddmState* state );
    void addBoundary( ddmBoundary* boundary, ddmCounty* county );
    void addVertex( const QSqlRecord& record, ddmBoundary* boundary );

    void prepareCache( bool force = false );
};

#endif // DDM_STATE_MODEL_H
