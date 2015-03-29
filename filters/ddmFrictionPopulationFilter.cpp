#include "filters/ddmFrictionPopulationFilter.h"
#include "models/ddmFrictionPopulationFilterModel.h"
#include "widgets/ddmFrictionPopulationFilterWidget.h"

#include <base/ddmCounty.h>

#include "ddmSettings.h"
#include "ddmInfoLogger.h"


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
    ddmInfoLogger& logger = ddmInfoLogger::instance();
    logger.writeInfo( "Выбран фильтр: Диапазон трений и населения");

    this->m_model = model;
    this->m_widget = widget;

    // Загружаем настройки
    this->loadSettings();

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
            // при установлении всех параметров (как в данной ситуации)
            // необходимо испустить сигнал только 1 раз (иначе лишние вызовы updateData)
            model->blockSignals( true );
            model->setFrBounds(  widget->minFrictionBound(),    widget->maxFrictionBound() );
            model->setPopBounds( widget->minPopulationBound(), widget->maxPopulationBound() );
            model->blockSignals( false );
            model->valueChanged();

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

            this->saveSettings();
        }
    }

}


void ddmFrictionPopulationFilter::updateSelection()
{
    int mult = 1000;
    ddmFilter::resetSelection();
    ddmFrictionPopulationFilterModel* model = this->model_cast<ddmFrictionPopulationFilterModel>();
    QVariantList counties = model->counties();
    ddmInfoLogger& logger = ddmInfoLogger::instance();
    logger.writeInfo( QObject::tr( "Отображаются графства с диапозоном населения от %1 до %2 человек и значениями трения от %3 до %4 " )
                     .arg( model->minPopBound() * mult ).arg( model->maxPopBound() * mult ).arg( model->minFrBound() ).arg( model->maxFrBound() ) );

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
void ddmFrictionPopulationFilter::loadSettings()
{
    ddmSettings* settings = ddmSettings::instance();

    double minFriction   =  settings->value( "ddmFrictionPopulationFilter/min_friction", 0.2 ).toDouble();
    double maxFriction   =  settings->value( "ddmFrictionPopulationFilter/max_friction", 20 ).toDouble();
    double minPopulation =  settings->value( "ddmFrictionPopulationFilter/min_population", 20.0 ).toDouble();
    double maxPopulation =  settings->value( "ddmFrictionPopulationFilter/max_population", 30.0 ).toDouble();

    ddmFrictionPopulationFilterModel* model  = this->model_cast<ddmFrictionPopulationFilterModel>();

    model->setFrBounds( minFriction, maxFriction );
    model->setPopBounds( minPopulation, maxPopulation );
}

/**
 * Сохраняет настройки фильтра
 *
 * @author  Цалко Т.В.
 * @since   2.7
 */
void ddmFrictionPopulationFilter::saveSettings()
{
    ddmSettings* settings = ddmSettings::instance();

    ddmFrictionPopulationFilterModel* model  = this->model_cast<ddmFrictionPopulationFilterModel>();
    settings->setValue( "ddmFrictionPopulationFilter/min_friction", model->minFrBound() );
    settings->setValue( "ddmFrictionPopulationFilter/max_friction", model->maxFrBound() );
    settings->setValue( "ddmFrictionPopulationFilter/min_population", model->minPopBound() );
    settings->setValue( "ddmFrictionPopulationFilter/max_population", model->maxPopBound() );
}

