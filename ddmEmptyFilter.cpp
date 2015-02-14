#include "ddmEmptyfilter.h"

#include "ddmMapView.h"
#include "ddmModel.h"
#include "ddmEmptyFlterWidget.h"


ddmEmptyFilter::ddmEmptyFilter( ddmModel *model, ddmMapView *view ): ddmFilter( model, view )
{
    m_widget = new ddmEmptyFilterWidget( this );
}


QWidget *ddmEmptyFilter::getWidget()
{
    return m_widget;
}


void ddmEmptyFilter::updateData()
{
    // TODO: обновление данных
}
