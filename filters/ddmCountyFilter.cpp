#include "filters/ddmCountyFilter.h"
#include "models/ddmCountyFilterModel.h"
#include "widgets/ddmCountyFilterWidget.h"

/**
 * @brief ddmCountyFilter::ddmCountyFilter
 * @param parent
 */
ddmCountyFilter::ddmCountyFilter( QObject* parent ) : ddmFilter( parent )
{
}

/**
 * @brief ddmCountyFilter::create
 */
void ddmCountyFilter::create()
{

    ddmCountyFilterModel* model = new ddmCountyFilterModel( this );
    ddmCountyFilterWidget* widget = new ddmCountyFilterWidget( this );

    this->m_model = model;
    this->m_widget = widget;

    QObject::connect( widget, SIGNAL( changedState()  ), this, SLOT( slotWidgetChangedState()  ) );
    QObject::connect( widget, SIGNAL( changedCounty() ), this, SLOT( slotWidgetChangedCounty() ) );

    widget->setStateNames( model->stateNames() );
    widget->setCurrentState( model->currentState()->geographicName() );
    widget->setCountyNames( model->currentState()->countyNames() );
    widget->setCurrentCounty( model->currentCounty()->geographicName() );

    // Обязательно вызываем метод из базового класса!
    ddmFilter::create();

    this->apply();

}

/**
 * @brief ddmCountyFilter::updateData
 * @param fromWidget
 */
void ddmCountyFilter::updateData( bool fromWidget )
{
    if ( this->isCreated() )
    {
        ddmCountyFilterModel*  model  = qobject_cast<ddmCountyFilterModel*>( this->model() );
        ddmCountyFilterWidget* widget = qobject_cast<ddmCountyFilterWidget*>( this->widget() );
        if ( fromWidget )
        {
            model->setCurrentState( widget->currentState() );
            model->setCurrentCounty( widget->currentCounty() );
        }
        else
        {
            widget->setCurrentState( model->currentState()->geographicName() );
            widget->setCurrentCounty( model->currentCounty()->geographicName() );
        }
    }
}

void ddmCountyFilter::updateSelection()
{
    ddmCountyFilterModel* model = qobject_cast<ddmCountyFilterModel*>( this->model() );

    bool needUpdate = false;
    QVariantList selection = this->selection();
    if ( selection.isEmpty() || selection.takeAt( 0 ).value<ddmCounty*>()->id() != model->currentCounty()->id() )
    {
        needUpdate = true;
    }

    if ( needUpdate )
    {
        this->resetSelection();
        model->currentCounty()->show();
        Q_EMIT selectionUpdated();
    }
}

void ddmCountyFilter::slotWidgetChangedState()
{
    ddmCountyFilterWidget* widget = qobject_cast<ddmCountyFilterWidget*>( this->widget() );
    QString stateName = widget->currentState();
    if ( !stateName.isEmpty() )
    {
        ddmCountyFilterModel* model = qobject_cast<ddmCountyFilterModel*>( this->model() );
        QStringList countyNames = model->state( stateName )->countyNames();
        widget->setCountyNames( countyNames );
    }
}

void ddmCountyFilter::slotWidgetChangedCounty()
{
    ddmCountyFilterWidget* widget = qobject_cast<ddmCountyFilterWidget*>( this->widget() );
    QString countyName = widget->currentCounty();
    if ( !countyName.isEmpty() )
    {
        ddmCountyFilterModel* model = qobject_cast<ddmCountyFilterModel*>( this->model() );
        model->setCurrentCounty( countyName );
        this->apply();

        QVariantMap center = model->currentCounty()->center();
        this->setMapCenter( center );
        widget->mapView()->setMarker( center );
    }
}

void ddmCountyFilter::slotMapLoaded()
{
    this->updateData();

    ddmCountyFilterModel* model = qobject_cast<ddmCountyFilterModel*>( this->model() );
    ddmCountyFilterWidget* widget = qobject_cast<ddmCountyFilterWidget*>( this->widget() );

    QVariantMap center = model->currentCounty()->center();
    this->setMapCenter( center );
    widget->mapView()->setMarker( center );
}

/**
 * @brief ddmCountyFilter::~ddmCountyFilter
 */
ddmCountyFilter::~ddmCountyFilter()
{
}
