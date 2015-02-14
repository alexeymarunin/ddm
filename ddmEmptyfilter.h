#ifndef DDMEMPTYFILTER
#define DDMEMPTYFILTER

#include <QStringList>
#include "ddmFilter.h"

class QWidget;
class ddmMapView;
class ddmModel;
class ddmEmptyFilterWidget;


class ddmEmptyFilter: public ddmFilter
{
public:

    ddmEmptyFilter( ddmModel* model, ddmMapView* view );

    virtual void updateData();

    virtual QWidget* getWidget();



private:
    ddmEmptyFilterWidget* m_widget;
};

#endif // DDMEMPTYFILTER

