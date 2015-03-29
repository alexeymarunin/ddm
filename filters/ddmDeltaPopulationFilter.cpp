#include "filters/ddmDeltaPopulationFilter.h"
#include "models/ddmDeltaPopulationFilterModel.h"
#include "widgets/ddmDeltaPopulationFilterWidget.h"

#include "base/ddmCounty.h"

#include "ddmSettings.h"
#include "ddmInfoLogger.h"


ddmDeltaPopulationFilter::ddmDeltaPopulationFilter( QObject *parent ) : ddmFilter( parent )
{


}


void ddmDeltaPopulationFilter::setup()
{
    ddmDeltaPopulationFilterModel* model = new ddmDeltaPopulationFilterModel( this );
    ddmDeltaPopulationFilterWidget* widget = new ddmDeltaPopulationFilterWidget( this );

    ddmInfoLogger& logger = ddmInfoLogger::instance();
    logger.writeInfo( "Выбран фильтр: Процент дельты от населения" );
    this->m_model = model;
    this->m_widget = widget;

    // Загружаем настройки
    this->loadSettings();

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

            this->saveSettings();
        }
    }
}


void ddmDeltaPopulationFilter::updateSelection()
{
    ddmFilter::resetSelection();
    ddmDeltaPopulationFilterModel* model = this->model_cast<ddmDeltaPopulationFilterModel>();
    QVariantList counties = model->counties();
    ddmInfoLogger& logger = ddmInfoLogger::instance();
    logger.writeInfo( QObject::tr( "Отображаются графства процент дельты от населения в которых: %2%-%3%" ).arg( model->minBound() * 100 ).arg( model->maxBound() * 100 ) );
    this->m_model = model;
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
    logger.writeInfo( QObject::tr( "Найдено %1 графств(а)" ).arg( counties.size() ) );
}


/**
 * Загружает настройки фильтра
 *
 * @author  Цалко Т.В.
 * @since   2.7
 */
void ddmDeltaPopulationFilter::loadSettings()
{
   ddmSettings* settings = ddmSettings::instance();
   ddmDeltaPopulationFilterModel* model  = this->model_cast<ddmDeltaPopulationFilterModel>();
   double minBound = settings->value( "ddmDeltaPopulationFilter/min_population", 0.05 ).toDouble();
   double maxBound = settings->value( "ddmDeltaPopulationFilter/max_population", 0.051 ).toDouble();
   model->setBounds( minBound, maxBound );

}


/**
 * Сохраняет настройки фильтра
 *
 * @author  Цалко Т.В.
 * @since   2.7
 */
void ddmDeltaPopulationFilter::saveSettings()
{
    ddmSettings* settings = ddmSettings::instance();

    ddmDeltaPopulationFilterModel* model  = this->model_cast<ddmDeltaPopulationFilterModel>();
    settings->setValue( "ddmDeltaPopulationFilter/min_population", model->minBound() );
    settings->setValue( "ddmDeltaPopulationFilter/max_population", model->maxBound() );
}


ddmDeltaPopulationFilter::~ddmDeltaPopulationFilter()
{

}
