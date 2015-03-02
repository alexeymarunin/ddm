#include "filters/ddmFilter.h"
#include "widgets/ddmFilterWidget.h"


/**
 * @brief ddmFilterWidget::ddmFilterWidget
 * @param filter
 * @param parent
 */
ddmFilterWidget::ddmFilterWidget( ddmFilter* filter, QWidget* parent ) : QWidget( parent ),
    m_filter( filter )
{
    this->setObjectName( "widget" ); // делаем виджет доступным в JavaScript
    this->setMinimumSize( 150, 300 );

    /**
     * @todo Если перенести mapView в класс фильтра,
     * то там можно сразу для него задать родителей centalWidget
     * Нафига оно вообще здесь?
     */
    this->m_mapView = new ddmMapView( filter );
}

/**
 * @brief ddmFilterWidget::filter
 * @return
 */
ddmFilter* ddmFilterWidget::filter() const
{
    return this->m_filter;
}

void ddmFilterWidget::slotApplyFilter()
{
    this->filter()->apply();
}

/**
 * @brief ddmFilterWidget::mapView
 * @return
 */
ddmMapView* ddmFilterWidget::mapView() const
{
    return this->m_mapView;
}

/**
 * @brief ddmFilterWidget::~ddmFilterWidget
 */
ddmFilterWidget::~ddmFilterWidget()
{
}
