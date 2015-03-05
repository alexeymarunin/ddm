#include <QtGlobal>
#include "base/ddmCounty.h"
#include "widgets/ddmCentralWidget.h"
#include "ddmMapView.h"
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
    this->m_mapView = new ddmMapView( this );
}

void ddmFilter::setup()
{
    Q_ASSERT( this->model() != NULL );
    Q_ASSERT( this->widget() != NULL );

    this->model()->reloadData();

    QObject::connect( this->model(),  SIGNAL( changed() ), this, SLOT( slotModelChanged()  ) );
    QObject::connect( this->widget(), SIGNAL( changed() ), this, SLOT( slotWidgetChanged() ) );

    ddmMapView* mapView = this->mapView();
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
    if ( !this->valid() )
    {
        this->setup();
    }

    this->widget()->show();
    this->mapView()->show();
}

void ddmFilter::deactivate()
{
    this->widget()->hide();
    this->mapView()->hide();
}

void ddmFilter::apply()
{
    if ( this->valid() )
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
        if ( value->visible() )
        {
            value->hide();
            updated = true;
        }
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
    if ( this->valid() )
    {
        this->mapView()->setCenter( center );
    }
}

void ddmFilter::setMapCenter( double x, double y )
{
    if ( this->valid() )
    {
        this->mapView()->setCenter( x, y );
    }
}

bool ddmFilter::isMapLoaded() const
{
    return this->mapView()->mapReady();
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
    this->mapView()->addToJavaScriptWindowObject( "ddmFilter", this );
}

bool ddmFilter::valid() const
{
    return ( this->model() && this->widget() && this->mapView() );
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
    return this->m_mapView;
}

/**
 * @brief ddmFilter::~ddmFilter
 */
ddmFilter::~ddmFilter()
{
}
