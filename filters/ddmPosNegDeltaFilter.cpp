#include <omp.h>

#include "filters/ddmPosNegDeltaFilter.h"
#include "widgets/ddmPosNegDeltaFilterWidget.h"
#include "models/ddmPosNegDeltaFilterModel.h"
#include <base/ddmCounty.h>


ddmPosNegDeltaFilter::ddmPosNegDeltaFilter( QObject *parent ) : ddmFilter( parent )
{

}


void ddmPosNegDeltaFilter::setup()
{
    ddmPosNegDeltaFilterWidget* widget = new ddmPosNegDeltaFilterWidget( this );
    ddmPosNegDeltaFilterModel*  model  = new ddmPosNegDeltaFilterModel( this );

    this->m_widget = widget;
    this->m_model  = model;

    model->setDeltaMode( widget->deltaMode() );

    // Обязательно вызываем метод из базового класса!
    ddmFilter::setup();

}


void ddmPosNegDeltaFilter::updateData( bool fromWidget )
{
    if ( this->valid() )
    {
        ddmPosNegDeltaFilterModel* model = this->model_cast<ddmPosNegDeltaFilterModel>();
        ddmPosNegDeltaFilterWidget* widget = this->widget_cast<ddmPosNegDeltaFilterWidget>();
        if ( fromWidget )
        {
            model->setDeltaMode( widget->deltaMode() );
        }
        else
        {
            widget->setDeltaMode( model->deltaMode() );
        }
    }
}


void ddmPosNegDeltaFilter::updateSelection()
{
    ddmFilter::resetSelection();
    ddmPosNegDeltaFilterModel* model = this->model_cast<ddmPosNegDeltaFilterModel>();
    QVariantList counties = model->counties();
    bool needUpdate = false;
    foreach ( QVariant obj, counties )
    {
        ddmCounty* county = obj.value<ddmCounty*>();
        if( !county->visible() )
        {
            county->show();
            needUpdate = true;
        }
    }

    if ( needUpdate )
    {
        Q_EMIT selectionUpdated();
    }
}

ddmPosNegDeltaFilter::~ddmPosNegDeltaFilter()
{
}


