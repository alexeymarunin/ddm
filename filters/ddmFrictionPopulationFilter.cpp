#include "filters/ddmFrictionPopulationFilter.h"
#include "models/ddmFrictionPopulationFilterModel.h"
#include "widgets/ddmFrictionPopulationFilterWidget.h"

#include <base/ddmCounty.h>



ddmFrictionPopulationFilter::ddmFrictionPopulationFilter( QObject *parent ) :
    ddmFilter( parent )
{

}


ddmFrictionPopulationFilter::~ddmFrictionPopulationFilter()
{
}


void ddmFrictionPopulationFilter::setup()
{
    ddmFrictionPopulationFilterModel*  model = new ddmFrictionPopulationFilterModel( this );
    ddmFrictionPopulationFilterWidget* widget = new ddmFrictionPopulationFilterWidget( this );

    this->m_model = model;
    this->m_widget = widget;

    // Задаем начальные значения
    model->setFrBounds( 0.2, 20.0 );
    model->setPopBounds( 20.0, 30.0 );

    // Обязательно вызываем метод из базового класса!
    ddmFilter::setup();
}


void ddmFrictionPopulationFilter::updateData( bool fromWidget )
{
    if ( this->valid() )
    {
        ddmFrictionPopulationFilterModel*  model  = this->model_cast<ddmFrictionPopulationFilterModel>();
        ddmFrictionPopulationFilterWidget* widget = this->widget_cast<ddmFrictionPopulationFilterWidget>();
        if ( fromWidget )
        {
            model->setFrBounds(  widget->minFrictionBound(),    widget->maxFrictionBound() );
            model->setPopBounds( widget->minPopulationBound(), widget->maxPopulationBound() );
        }
        else
        {
            widget->setMinFiction( model->minFriction() );
            widget->setMaxFiction( model->maxFriction() );

            widget->setMinFrictionBound( model->minFrBound() );
            widget->setMaxFrictionBound( model->maxFrBound() );

            widget->setMinPopulation( model->minPopulation() );
            widget->setMaxPopulation( model->maxPopulation() );

            widget->setMinPopulationBound( model->minPopBound() );
            widget->setMaxPopulationBound( model->maxPopBound() );
        }
    }

}


void ddmFrictionPopulationFilter::updateSelection()
{
    ddmFilter::resetSelection();
    ddmFrictionPopulationFilterModel* model = this->model_cast<ddmFrictionPopulationFilterModel>();
    QVariantList counties = model->counties();
    bool needUpdate = false;
    foreach ( QVariant obj, counties )
    {
        ddmCounty* county = obj.value<ddmCounty*>();
        if ( !county->visible() )
        {
            county->show();
            needUpdate = true;
        }
    }


    if ( needUpdate )
        Q_EMIT selectionUpdated();
}
