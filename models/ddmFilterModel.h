#ifndef DDM_FILTER_MODEL_H
#define DDM_FILTER_MODEL_H

#include <QMap>
#include "models/ddmStateModel.h"

typedef QMap<int, ddmState*> ddmStateMap;
typedef QMap<int, ddmCounty*> ddmCountyMap;
typedef QMap<int, ddmBoundary*> ddmBoundaryMap;

/**
 * Класс ddmFilterModel является базовым классом модели любого фильтра
 * Предоставляет доступ к модели фильтра в JavaScript (через поле model)
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
class ddmFilterModel : public ddmStateModel
{
    Q_OBJECT

public:

    ddmFilterModel( QObject* parent = 0 );

    virtual void reloadData();
    virtual void execQuery( const QString& sqlQuery );

    ddmStateMap& totalStates() const;
    ddmCountyMap& totalCounties() const;
    ddmBoundaryMap& totalBoundaries() const;

    virtual ~ddmFilterModel();

protected:

    ddmStateMap     m_totalStates;
    ddmCountyMap    m_totalCounties;
    ddmBoundaryMap  m_totalBoundaries;

    ddmState* findState( int state_id ) const;
    ddmState* findState( const QString& state_name ) const;
    ddmCounty* findCounty( int county_id ) const;
    ddmCounty* findCounty( const QString& county_name ) const;
    ddmBoundary* findBoundary( int boundary_id ) const;

};

#endif // DDM_FILTER_MODEL_H
