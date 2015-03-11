#include "filters/ddmFilter.h"
#include "ddmApplication.h"
#include "widgets/ddmCentralWidget.h"
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

    if ( !parent )
    {
        this->setParent( ddmApp->centralWidget() );
    }
    this->m_widgetLayout = ddmApp->centralWidget()->widgetLayout();
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

void ddmFilterWidget::show()
{
    QWidget::show();
    this->m_widgetLayout->addWidget( this );
}

void ddmFilterWidget::hide()
{
    QWidget::hide();
    this->m_widgetLayout->removeWidget( this );
}


/**
 * @brief ddmFilterWidget::~ddmFilterWidget
 */
ddmFilterWidget::~ddmFilterWidget()
{
}
