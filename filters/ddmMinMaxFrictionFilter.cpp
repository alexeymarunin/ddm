#include "base/ddmCounty.h"
#include "models/ddmMinMaxFrictionFilterModel.h"
#include "widgets/ddmMinMaxFrictionFilterWidget.h"
#include "filters/ddmMinMaxFrictionFilter.h"

ddmMinMaxFrictionFilter::ddmMinMaxFrictionFilter( QObject* parent ) : ddmFilter( parent )
{
}

void ddmMinMaxFrictionFilter::setup()
{
    ddmMinMaxFrictionFilterModel* model = new ddmMinMaxFrictionFilterModel( this );
    ddmMinMaxFrictionFilterWidget* widget = new ddmMinMaxFrictionFilterWidget( this );

    this->m_model = model;
    this->m_widget = widget;

    // Обязательно вызываем метод из базового класса!
    ddmFilter::setup();

    // Задаем ограничения
    model->setMinBound( model->minFriction() );
    model->setMaxBound( 0.04 );

    this->updateData( false );

    this->apply();
}

void ddmMinMaxFrictionFilter::updateData( bool fromWidget )
{
    if ( this->valid() )
    {
        ddmMinMaxFrictionFilterModel* model = this->model_cast<ddmMinMaxFrictionFilterModel>();
        ddmMinMaxFrictionFilterWidget* widget = this->widget_cast<ddmMinMaxFrictionFilterWidget>();
        if ( fromWidget )
        {
            model->setBounds( widget->minBound(), widget->maxBound() );
        }
        else
        {
            widget->setMinFiction( model->minFriction() );
            widget->setMaxFiction( model->maxFriction() );

            widget->setMinBound( model->minBound() );
            widget->setMaxBound( model->maxBound() );
        }
    }
}

void ddmMinMaxFrictionFilter::updateSelection()
{
    ddmMinMaxFrictionFilterModel* model = this->model_cast<ddmMinMaxFrictionFilterModel>();
    QVariantList counties = model->counties();
    double minBound = model->minBound();
    double maxBound = model->maxBound();

    bool needUpdate = false;
    foreach ( QVariant obj, counties )
    {
        ddmCounty* county = obj.value<ddmCounty*>();
        double f_mid = county->f_mid();
        if ( f_mid >= minBound && f_mid <= maxBound )
        {
            if ( !county->visible() )
            {
                county->show();
                needUpdate = true;
            }
            // TODO: здесь должны быть цветовые настройки
        }
        else
        {
            if ( county->visible() )
            {
                county->hide();
                needUpdate = true;
            }
        }
    }

    if ( needUpdate )
    {
        Q_EMIT selectionUpdated();
    }
}

ddmMinMaxFrictionFilter::~ddmMinMaxFrictionFilter()
{
}


