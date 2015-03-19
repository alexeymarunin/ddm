#ifndef DDM_NEIGHBOR_COUNTY_FILTER_MODEL_H
#define DDM_NEIGHBOR_COUNTY_FILTER_MODEL_H

#include <QSet>
#include "models/ddmBaseCountyFilterModel.h"

/**
 * Класс ddmNeighborCountyFilterModel определяет модель фильтра ddmNeighborCountyFilter
 * Отображает соседей первого порядка для выбранного графства
 *
 * Основными элементами модели являются:
 *  - neighbors список идентификаторов соседей
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
class ddmNeighborCountyFilterModel : public ddmBaseCountyFilterModel
{
    Q_OBJECT

public:

    ddmNeighborCountyFilterModel( QObject* parent = 0 );

    virtual void reloadData();

    QSet<int> neighbors() const;

    virtual ~ddmNeighborCountyFilterModel();

protected:
    QSet<int> m_neighbors;

    void updateNeighbors();
};

#endif // DDM_NEIGHBOR_COUNTY_FILTER_MODEL_H
