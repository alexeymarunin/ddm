#include <QWidget>

#include "ddmMigrationCountyFilter.h"
#include "ddmMigrationCountyFilterWidget.h"


ddmMigrationCountyFilter::ddmMigrationCountyFilter( ddmModel *model, ddmMapView *view ):
     ddmFilter( model, view )
{
    m_widget = new ddmMigrationCountyFilterWidget( this );
}


void ddmMigrationCountyFilter::updateData()
{

}


QWidget *ddmMigrationCountyFilter::getWidget()
{
    return m_widget;
}
