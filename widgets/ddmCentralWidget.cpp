#include <QResizeEvent>
#include <QWebFrame>
#include <QComboBox>
#include <QVBoxLayout>

#include "ddmMapView.h"
#include "widgets/ddmCentralWidget.h"
#include "ui_ddmCentralWidget.h"

#include "filters/ddmFilter.h"

/**
 * @brief ddmCentralWidget::ddmCentralWidget
 * @param parent
 */
ddmCentralWidget::ddmCentralWidget( QWidget* parent ) : QWidget( parent ),
    m_currentFilter( NULL )
{
    this->ui = new Ui::ddmCentralWidget;
    this->ui->setupUi( this );

    QObject::connect( this->comboFilter(), SIGNAL( currentIndexChanged(int) ), this, SLOT( slotChangedFilter(int) ) );

}

void ddmCentralWidget::appendFilter( const QString& title, ddmFilter* filter )
{
    QComboBox* comboFilter = this->comboFilter();
    comboFilter->addItem( title, QVariant::fromValue( filter ) );

}

void ddmCentralWidget::setCurrentFilter( int index )
{
    int currentIndex = this->comboFilter()->currentIndex();
    if ( currentIndex != index )
    {
        this->comboFilter()->setCurrentIndex( index );
    }
    else
    {
        this->slotChangedFilter( index );
    }
}

QComboBox* ddmCentralWidget::comboFilter() const
{
    return this->ui->comboFilter;
}

void ddmCentralWidget::slotChangedFilter( int index )
{
    QComboBox* comboFilter = this->comboFilter();
    ddmFilter* filter = comboFilter->itemData( index ).value<ddmFilter*>();

    if ( this->currentFilter() != filter )
    {
        if ( this->currentFilter() )
        {
            this->currentFilter()->deactivate();
            this->m_currentFilter = NULL;
        }
        if ( filter )
        {
            this->m_currentFilter = filter;
            filter->activate();
        }
    }
}

ddmFilter* ddmCentralWidget::currentFilter() const
{
    return this->m_currentFilter;
}

void ddmCentralWidget::resizeEvent( QResizeEvent* event )
{
    QSize size = event->size();
    if ( this->currentFilter() )
    {
        this->currentFilter()->mapView()->resize( size.width(), size.height() );
    }
}

void ddmCentralWidget::reload()
{
    if ( this->currentFilter() )
    {
        this->currentFilter()->mapView()->reload();
    }
}


void ddmCentralWidget::increaseZoom()
{
    if ( this->currentFilter() )
        this->currentFilter()->mapView()->increaseZoomLevel();
}


void ddmCentralWidget::decreaseZoom()
{
    if ( this->currentFilter() )
        this->currentFilter()->mapView()->decreaseZoomLevel();
}

QVBoxLayout* ddmCentralWidget::widgetLayout() const
{
    return this->ui->widgetLayout;
}

QVBoxLayout* ddmCentralWidget::mapLayout() const
{
    return this->ui->mapLayout;
}

ddmCentralWidget::~ddmCentralWidget()
{
    delete this->ui;
}
