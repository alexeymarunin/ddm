#include "ddmFilter.h"

#include <QWebFrame>
#include "ddmMapView.h"
#include "ddmModel.h"


ddmFilter::ddmFilter( ddmModel* model, ddmMapView* view )
{
    m_model    = model;
    m_mapView  = view;
}


ddmModel *ddmFilter::model()
{
    return m_model;
}


ddmMapView *ddmFilter::mapView()
{
    return m_mapView;
}


void ddmFilter::visualize()
{
    // TODO: код обновления визуализации
}
