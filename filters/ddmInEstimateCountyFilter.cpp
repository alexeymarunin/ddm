#include "filters/ddmInEstimateCountyFilter.h"
#include "models/ddmInEstimateCountyFilterModel.h"
#include "widgets/ddmInEstimateCountyFilterWidget.h"

#include "ddmInfoLogger.h"
#include "ddmSettings.h"


ddmInEstimateCountyFilter::ddmInEstimateCountyFilter( QObject *parent ):
    ddmBaseCountyFilter( parent )
{

}


void ddmInEstimateCountyFilter::setup()
{
    ddmInEstimateCountyFilterModel* model = new ddmInEstimateCountyFilterModel( this );
    ddmInEstimateCountyFilterWidget* widget = new ddmInEstimateCountyFilterWidget( this );
    ddmInfoLogger& logger = ddmInfoLogger::instance();
    logger.writeInfo( "Выбран фильтр: Миграция в графство" );

    this->m_model = model;
    this->m_widget = widget;

    this->loadSettings();

    ddmBaseCountyFilter::setup();

}

void ddmInEstimateCountyFilter::updateData( bool fromWidget )
{
    if ( this->valid() )
    {
        ddmInEstimateCountyFilterModel*  model  = this->model_cast<ddmInEstimateCountyFilterModel>();
        ddmInEstimateCountyFilterWidget* widget = this->widget_cast<ddmInEstimateCountyFilterWidget>();
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


void ddmInEstimateCountyFilter::updateSelection()
{
    ddmInEstimateCountyFilterModel* model  = this->model_cast<ddmInEstimateCountyFilterModel>();
    this->resetSelection();
    int mult = 1000;
    ddmInfoLogger& logger = ddmInfoLogger::instance();
    logger.writeInfo( QObject::tr( "Отобржаются графства  из которых приехало от %1 до %2 человек" )
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

    logger.writeInfo( QObject::tr( "Отображено %1 графств" ).arg( model->counties().size() ) );
    Q_EMIT selectionUpdated();
}


ddmInEstimateCountyFilter::~ddmInEstimateCountyFilter()
{

}


void ddmInEstimateCountyFilter::saveSettings()
{
    ddmSettings* settings = ddmSettings::instance();

    ddmBaseCountyFilter::saveSettings();

    ddmInEstimateCountyFilterModel* model  = this->model_cast<ddmInEstimateCountyFilterModel>();
    settings->setValue( "ddmInEstimateCountyFilter/min_population", model->minPopBound() );
    settings->setValue( "ddmInEstimateCountyFilter/max_population", model->maxPopBound() );
}


void ddmInEstimateCountyFilter::loadSettings()
{
    ddmSettings* settings = ddmSettings::instance();

    ddmBaseCountyFilter::loadSettings();

    ddmInEstimateCountyFilterModel* model  = this->model_cast<ddmInEstimateCountyFilterModel>();
    double minBound = settings->value( "ddmInEstimateCountyFilter/min_population", 0.001 ).toDouble();
    double maxBound = settings->value( "ddmInEstimateCountyFilter/max_population", 0.1 ).toDouble();
    model->setPopBounds( minBound, maxBound );
}
