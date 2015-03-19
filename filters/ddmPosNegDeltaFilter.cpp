#include "ddmSettings.h"

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

    // Загружаем настройки
    this->loadSettings();

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
            this->saveSettings();
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
        Q_EMIT selectionUpdated();
}


/**
 * Загружает настройки фильтра
 *
 * @author  Цалко Т.В.
 * @since   2.7
 */
void ddmPosNegDeltaFilter::loadSettings()
{
    ddmSettings* settings = ddmSettings::instance();

    int deltaMode = settings->value( "ddmPosNegDeltaFilter/mode", 0 ).toInt();
    ddmPosNegDeltaFilterModel* model  = this->model_cast<ddmPosNegDeltaFilterModel>();
    model->setDeltaMode( deltaMode );

}


/**
 * Сохраняет настройки фильтра
 *
 * @author  Цалко Т.В.
 * @since   2.7
 */
void ddmPosNegDeltaFilter::saveSettings()
{
    ddmSettings* settings = ddmSettings::instance();
    ddmPosNegDeltaFilterModel* model  = this->model_cast< ddmPosNegDeltaFilterModel>();
    settings->setValue( "ddmPosNegDeltaFilter/mode", model->deltaMode() );
}


ddmPosNegDeltaFilter::~ddmPosNegDeltaFilter()
{
}


