#ifndef DDM_NEIGHBOR_COUNTY_FILTER_H
#define DDM_NEIGHBOR_COUNTY_FILTER_H

#include "filters/ddmBaseCountyFilter.h"

/**
 * Класс ddmNeighborCountyFilter описывает фильтр для отображения одного графства
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
class ddmNeighborCountyFilter : public ddmBaseCountyFilter
{
    Q_OBJECT

public:

    ddmNeighborCountyFilter( QObject* parent = 0 );

    virtual ~ddmNeighborCountyFilter();

protected:

    virtual void setup();
    virtual void updateSelection();

};

#endif // DDM_COUNTY_FILTER_H
