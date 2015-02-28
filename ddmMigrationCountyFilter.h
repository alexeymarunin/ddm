#ifndef DDMMIGRATIONCOUNTYFILTER_H
#define DDMMIGRATIONCOUNTYFILTER_H

#include "ddmFilter.h"

class ddmMigrationCountyFilterWidget;


class ddmMigrationCountyFilter: public ddmFilter
{
public:

    ddmMigrationCountyFilter( ddmModel* model, ddmMapView* view );

    virtual void updateData();

    virtual QWidget* getWidget();

private:
    ddmMigrationCountyFilterWidget* m_widget;
};

#endif // DDMMIGRATIONCOUNTYFILTER_H

