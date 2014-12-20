#ifndef DDM_COUNTY_H
#define DDM_COUNTY_H

#include <QString>
#include <QPointF>
#include "ddmState.h"
#include "ddmBoundary.h"

class ddmModel;

typedef QMap<int, ddmBoundary*> ddmBoundaryMap;

class ddmCounty : public ddmObject
{
    Q_OBJECT
public:
    explicit ddmCounty( int id = 0, ddmModel* model = 0 );

    void addBoundary( ddmBoundary* boundary );
    ddmBoundary* boundary( int id ) const;
    ddmBoundaryMap& boundaries() const;

    void setState( ddmState* state );
    ddmState* state() const;

    void setCenter( const QPointF& point );
    QPointF center() const;

    void setGeographicName( const QString& name );
    QString geographicName() const;

    void setModel( ddmModel* model );
    ddmModel* model() const;

    virtual ~ddmCounty();

signals:

public slots:

private:
    QString         m_geographicName;
    QPointF         m_center;
    ddmBoundaryMap  m_boundaries;
    ddmState*       m_state;

};

#endif // DDM_COUNTY_H
