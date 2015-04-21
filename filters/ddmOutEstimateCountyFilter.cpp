#include "filters/ddmOutEstimateCountyFilter.h"
#include "models/ddmOutEstimateCountyFilterModel.h"
#include "widgets/ddmOutEstimateCountyFilterWidget.h"

#include "ddmInfoLogger.h"
#include "ddmSettings.h"


ddmOutEstimateCountyFilter::ddmOutEstimateCountyFilter( QObject *parent ):
    ddmBaseCountyFilter( parent )
{

}


void ddmOutEstimateCountyFilter::setup()
{
    ddmOutEstimateCountyFilterModel* model = new ddmOutEstimateCountyFilterModel( this );
    ddmOutEstimateCountyFilterWidget* widget = new ddmOutEstimateCountyFilterWidget( this );
    ddmInfoLogger& logger = ddmInfoLogger::instance();
    logger.writeInfo( "Выбран фильтр: Миграция из графства" );

    this->m_model = model;
    this->m_widget = widget;

    this->loadSettings();

    ddmBaseCountyFilter::setup();

}

void ddmOutEstimateCountyFilter::updateData( bool fromWidget )
{
    if ( this->valid() )
    {
        ddmOutEstimateCountyFilterModel*  model  = this->model_cast<ddmOutEstimateCountyFilterModel>();
        ddmOutEstimateCountyFilterWidget* widget = this->widget_cast<ddmOutEstimateCountyFilterWidget>();
        if ( fromWidget )
        {
            model->setPopBounds( widget->minPopulationBound(), widget->maxPopulationBound() );
        }
        else
        {
            widget->setMinPopulation( model->minPopulation() );
            widget->setMaxPopulation( model->maxPopulation() );

            widget->setMinPopulationBound( model->minPopBound() );
            widget->setMaxPopulationBound( model->maxPopBound() );

            this->saveSettings();        }
    }
    ddmBaseCountyFilter::updateData( fromWidget );

}


void ddmOutEstimateCountyFilter::updateSelection()
{
    ddmOutEstimateCountyFilterModel* model  = this->model_cast<ddmOutEstimateCountyFilterModel>();
    this->resetSelection();
    int mult = 1000;
    ddmInfoLogger& logger = ddmInfoLogger::instance();
    logger.writeInfo( QObject::tr( "Отображаются графства, в которые уехало от %1 до %2 человек" )
                     .arg( model->minPopBound() * mult ).arg( model->maxPopBound() * mult ) );

    ddmCounty* currentCounty = model->currentCounty();
    currentCounty->show();
    currentCounty->unselect();

    foreach ( int county_id, model->counties() )
    {
        ddmCounty* county = model->county( county_id );
        county->show();
        county->select();
    }

    logger.writeInfo( QObject::tr( "Найдено %1 графств" ).arg( model->counties().size() ) );
    Q_EMIT selectionUpdated();
}


void ddmOutEstimateCountyFilter::saveSettings()
{
    ddmSettings* settings = ddmSettings::instance();

    ddmBaseCountyFilter::saveSettings();

    ddmOutEstimateCountyFilterModel* model  = this->model_cast<ddmOutEstimateCountyFilterModel>();
    settings->setValue( "ddmOutEstimateCountyFilter/min_population", model->minPopBound() );
    settings->setValue( "ddmOutEstimateCountyFilter/max_population", model->maxPopBound() );
}


void ddmOutEstimateCountyFilter::loadSettings()
{
    ddmSettings* settings = ddmSettings::instance();

    ddmBaseCountyFilter::loadSettings();

    ddmOutEstimateCountyFilterModel* model  = this->model_cast<ddmOutEstimateCountyFilterModel>();
    double minBound = settings->value( "ddmOutEstimateCountyFilter/min_population", 0.001 ).toDouble();
    double maxBound = settings->value( "ddmOutEstimateCountyFilter/max_population", 0.1 ).toDouble();
    model->setPopBounds( minBound, maxBound );
}


ddmOutEstimateCountyFilter::~ddmOutEstimateCountyFilter()
{

}
