#ifndef DDM_DELTA_POPULATION_FILTER_H
#define DDMDELTAPOPULATIONFILTER

#include "filters/ddmFilter.h"


class ddmDeltaPopulationFilter: public ddmFilter
{
    Q_OBJECT

public:

    ddmDeltaPopulationFilter( QObject* parent = NULL );

    virtual ~ddmDeltaPopulationFilter();

protected:

    virtual void setup();
    virtual void updateData( bool fromWidget = true );
    virtual void updateSelection();

    virtual void loadSettings();
    virtual void saveSettings();

};

#endif // DDMDELTAPOPULATIONFILTER

