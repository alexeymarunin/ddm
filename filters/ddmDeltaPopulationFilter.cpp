#include "filters/ddmDeltaPopulationFilter.h"
#include "models/ddmDeltaPopulationFilterModel.h"
#include "widgets/ddmDeltaPopulationFilterWidget.h"

#include <base/ddmCounty.h>



ddmDeltaPopulationFilter::ddmDeltaPopulationFilter( QObject *parent ) : ddmFilter( parent )
{


}


void ddmDeltaPopulationFilter::setup()
{
    ddmDeltaPopulationFilterModel* model = new ddmDeltaPopulationFilterModel( this );
    ddmDeltaPopulationFilterWidget* widget = new ddmDeltaPopulationFilterWidget( this );

    this->m_model = model;
    this->m_widget = widget;

    // Задаем начальные значения
    model->setBounds( 0.05, 0.051 );

    // Обязательно вызываем метод из базового класса!
    ddmFilter::setup();
}

void ddmDeltaPopulationFilter::updateData( bool fromWidget )
{
    if ( this->valid() )
    {
        ddmDeltaPopulationFilterModel* model = this->model_cast<ddmDeltaPopulationFilterModel>();
        ddmDeltaPopulationFilterWidget* widget = this->widget_cast<ddmDeltaPopulationFilterWidget>();
        if ( fromWidget )
        {
            model->setBounds( widget->minBound(), widget->maxBound() );
        }
        else
        {
            widget->setMinPopulation( model->minPopulation() );
            widget->setMaxPopulation( model->maxPopulation() );

            widget->setMinBound( model->minBound() );
            widget->setMaxBound( model->maxBound() );
        }
    }
}


void ddmDeltaPopulationFilter::updateSelection()
{
    ddmFilter::resetSelection();
    ddmDeltaPopulationFilterModel* model = this->model_cast<ddmDeltaPopulationFilterModel>();
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


ddmDeltaPopulationFilter::~ddmDeltaPopulationFilter()
{

}
