#include "ddmFrictionCountyFilter.h"

#include "ddmFrictionCountyFilterWidget.h"
#include "ddmMapView.h"
#include "ddmModel.h"


ddmFrictionCountyFilter::ddmFrictionCountyFilter( ddmModel *model, ddmMapView *view ): ddmFilter( model, view )
{
    m_widget = new ddmFrictionCountyFilterWidget( this );
}


void ddmFrictionCountyFilter::updateData()
{

}


QWidget *ddmFrictionCountyFilter::getWidget()
{
    return m_widget;
}
