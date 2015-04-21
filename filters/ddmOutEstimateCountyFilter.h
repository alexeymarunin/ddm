#ifndef DDM_OUT_ESTIMATE_COUNTY_FILTER_H
#define DDM_OUT_ESTIMATE_COUNTY_FILTER_H

#include "filters/ddmBaseCountyFilter.h"


class ddmOutEstimateCountyFilter : public ddmBaseCountyFilter
{
    Q_OBJECT

public:

    ddmOutEstimateCountyFilter( QObject* parent = 0 );

    virtual ~ddmOutEstimateCountyFilter();

    virtual void saveSettings();
    virtual void loadSettings();

protected Q_SLOTS:

    virtual void slotWidgetChangedState();
    virtual void slotWidgetChangedCounty();

protected:

    virtual void setup();
    virtual void updateData( bool fromWidget = true );
    virtual void updateSelection();

};

#endif // DDM_OUT_ESTIMATE_COUNTY_FILTER_H

