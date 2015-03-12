#ifndef DDM_FRICTION_POPULATION_FILTER_H
#define DDM_FRICTION_POPULATION_FILTER_H

#include "filters/ddmFilter.h"


class ddmFrictionPopulationFilter : public ddmFilter
{
    Q_OBJECT

public:

    ddmFrictionPopulationFilter( QObject* parent = NULL );

    virtual ~ddmFrictionPopulationFilter();

protected:

    virtual void setup();
    virtual void updateData( bool fromWidget = true );
    virtual void updateSelection();

};

#endif // DDM_FRICTION_POPULATION_FILTER_H

