#ifndef DDM_IN_ESTIMATE_COUNTY_FILTER_H
#define DDM_IN_ESTIMATE_COUNTY_FILTER_H

#include "filters/ddmBaseCountyFilter.h"


class ddmInEstimateCountyFilter : public ddmBaseCountyFilter
{
    Q_OBJECT

public:

    ddmInEstimateCountyFilter( QObject* parent = 0 );

    virtual ~ddmInEstimateCountyFilter();

    virtual void saveSettings();
    virtual void loadSettings();

protected:

    virtual void setup();
    virtual void updateData( bool fromWidget = true );
    virtual void updateSelection();

};

#endif // DDM_IN_ESTIMATE_COUNTY_FILTER_H

