#include <QtGlobal>
#include "base/ddmCounty.h"
#include "widgets/ddmCentralWidget.h"
#include "ddmApplication.h"
#include "filters/ddmFilter.h"


/**
 * @brief ddmFilter::ddmFilter
 * @param parent
 */
ddmFilter::ddmFilter( QObject* parent ) : ddmObject( parent ),
    m_model( NULL ), m_widget( NULL )
{
    this->setObjectName( "ddmFilter" );
}

void ddmFilter::create()
{
    Q_ASSERT( this->model() != NULL );
    Q_ASSERT( this->widget() != NULL );

    this->model()->load();

    QObject::connect( this->model(),  SIGNAL( changed() ), this, SLOT( slotModelChanged()  ) );
    QObject::connect( this->widget(), SIGNAL( changed() ), this, SLOT( slotWidgetChanged() ) );

    ddmMapView* mapView = this->widget()->mapView();
    QObject::connect( mapView, SIGNAL( javaScriptWindowObjectCleared() ), this, SLOT( slotJavaScriptWindowObjectCleared() ) );
    QObject::connect( mapView, SIGNAL( loaded() ), this, SLOT( slotMapLoaded() ) );

    this->updateData( false );
}

void ddmFilter::updateData( bool fromWidget )
{
    Q_UNUSED( fromWidget )
}

void ddmFilter::updateSelection()
{
}

void ddmFilter::activate()
{
    if ( !this->isCreated() )
    {
        this->create();
    }

    ddmCentralWidget* centralWidget = ddmApp->centralWidget();
    QVBoxLayout* widgetLayout = centralWidget->widgetLayout();
    QVBoxLayout* mapLayout = centralWidget->mapLayout();

    ddmFilterWidget* filterWidget = this->widget();
    if ( !filterWidget->parentWidget() ) filterWidget->setParent( centralWidget );
    widgetLayout->addWidget( filterWidget );
    filterWidget->show();

    ddmMapView* filterMap = this->mapView();
    if ( !filterMap->parentWidget() ) filterMap->setParent( centralWidget );
    mapLayout->addWidget( filterMap );
    filterMap->show();
}

void ddmFilter::deactivate()
{
    ddmCentralWidget* centralWidget = ddmApp->centralWidget();
    QVBoxLayout* widgetLayout = centralWidget->widgetLayout();
    QVBoxLayout* mapLayout = centralWidget->mapLayout();

    ddmFilterWidget* filterWidget = this->widget();
    widgetLayout->removeWidget( filterWidget );
    filterWidget->hide();

    ddmMapView* filterMap = this->mapView();
    mapLayout->removeWidget( filterMap );
    filterMap->hide();
}

void ddmFilter::apply()
{
    if ( this->isCreated() )
    {
        this->updateData();
        this->updateSelection();
    }
}

void ddmFilter::resetSelection()
{
    QVariantList selection = this->selection();
    bool updated = false;
    foreach ( QVariant obj, selection )
    {
        ddmCounty* value = obj.value<ddmCounty*>();
        value->hide();
        updated = true;
    }

    if ( updated )
    {
        Q_EMIT selectionUpdated();
    }
}

QVariantList ddmFilter::selection() const
{
    QVariantList selection;
    QVariantList counties = this->model()->counties();
    foreach ( QVariant obj, counties )
    {
        ddmCounty* value = obj.value<ddmCounty*>();
        if ( value->visible() )
        {
            selection.append( obj );
        }
    }

    return selection;
}

void ddmFilter::setMapCenter( const QVariantMap& center )
{
    if ( this->isCreated() )
    {
        this->widget()->mapView()->setCenter( center );
    }
}

void ddmFilter::setMapCenter( double x, double y )
{
    if ( this->isCreated() )
    {
        this->widget()->mapView()->setCenter( x, y );
    }
}

bool ddmFilter::isMapLoaded() const
{
    return ( this->mapView() && this->mapView()->mapReady() );
}

void ddmFilter::slotModelChanged()
{
    this->updateData( false );
    // this->apply();
}

void ddmFilter::slotWidgetChanged()
{
    // this->apply();
}

void ddmFilter::slotMapLoaded()
{
}

void ddmFilter::slotJavaScriptWindowObjectCleared()
{
    this->widget()->mapView()->addToJavaScriptWindowObject( "ddmFilter", this );
}

bool ddmFilter::isCreated() const
{
    return ( this->model() && this->widget() );
}

/**
 * @brief ddmFilter::model
 * @return
 */
ddmFilterModel* ddmFilter::model() const
{
    return this->m_model;
}

/**
 * @brief ddmFilter::widget
 * @return
 */
ddmFilterWidget* ddmFilter::widget() const
{
    return this->m_widget;
}

ddmMapView* ddmFilter::mapView() const
{
    return ( this->isCreated() ? this->widget()->mapView() : NULL );
}

/**
 * @brief ddmFilter::~ddmFilter
 */
ddmFilter::~ddmFilter()
{
}
