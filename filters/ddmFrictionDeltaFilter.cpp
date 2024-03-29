#include "base/ddmCounty.h"

#include "filters/ddmFrictionDeltaFilter.h"
#include "models/ddmFrictionDeltaFilterModel.h"
#include "widgets/ddmFrictionDeltaFilterWidget.h"


ddmFrictionDeltaFilter::ddmFrictionDeltaFilter( QObject* parent ) : ddmFilter( parent )
{

}

void ddmFrictionDeltaFilter::setup()
{
    ddmFrictionDeltaFilterModel* model = new ddmFrictionDeltaFilterModel( this );
    ddmFrictionDeltaFilterWidget* widget = new ddmFrictionDeltaFilterWidget( this );

    this->m_model = model;
    this->m_widget = widget;

    // Задаем ограничения
    model->setBounds( 0.02, 0.04 );
    // задаем режим дельты (положительная/отрицательная)
    model->setDeltaMode( widget->deltaMode() );

    // Обязательно вызываем метод из базового класса!
    ddmFilter::setup();
}


void ddmFrictionDeltaFilter::updateData( bool fromWidget )
{
    if ( this->valid() )
    {
        ddmFrictionDeltaFilterModel* model = this->model_cast<ddmFrictionDeltaFilterModel>();
        ddmFrictionDeltaFilterWidget* widget = this->widget_cast<ddmFrictionDeltaFilterWidget>();
        if ( fromWidget )
        {
            model->setBounds( widget->minBound(), widget->maxBound() );
            model->setDeltaMode( widget->deltaMode() );
        }
        else
        {
            widget->setMinFiction( model->minFriction() );
            widget->setMaxFiction( model->maxFriction() );

            widget->setMinBound( model->minBound() );
            widget->setMaxBound( model->maxBound() );

            widget->setDeltaMode( model->deltaMode() );
        }
    }
}


void ddmFrictionDeltaFilter::updateSelection()
{
    ddmFilter::resetSelection();
    ddmFrictionDeltaFilterModel* model = this->model_cast<ddmFrictionDeltaFilterModel>();
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

ddmFrictionDeltaFilter::~ddmFrictionDeltaFilter()
{

}


