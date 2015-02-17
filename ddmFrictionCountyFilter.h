#ifndef DDMFRICTIONCOUNTYFILTER
#define DDMFRICTIONCOUNTYFILTER

#include "ddmFilter.h"

class QWidget;
class ddmMapView;
class ddmModel;
class ddmFrictionCountyFilterWidget;

class ddmFrictionCountyFilter: public ddmFilter
{

public:
    ddmFrictionCountyFilter( ddmModel* model, ddmMapView* view );

    virtual void updateData();

    virtual QWidget *getWidget();

private:
    ddmFrictionCountyFilterWidget* m_widget;

};

#endif // DDMFRICTIONCOUNTYFILTER

