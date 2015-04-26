#include "filters/ddmOutEstimateCountyFilter.h"
#include "models/ddmOutEstimateCountyFilterModel.h"
#include "widgets/ddmOutEstimateCountyFilterWidget.h"

#include "ddmMapView.h"

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

    QObject::connect( widget, SIGNAL( changedState()  ), this, SLOT( slotWidgetChangedState()  ), Qt::UniqueConnection );
    QObject::connect( widget, SIGNAL( changedCounty() ), this, SLOT( slotWidgetChangedCounty() ), Qt::UniqueConnection );

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
        this->drawArrow( currentCounty->countyCenter(), county->countyCenter(), 3.0 );
        county->show();
        county->select();
    }

    logger.writeInfo( QObject::tr( "Найдено %1 графств" ).arg( model->counties().size() ) );
    Q_EMIT selectionUpdated();
}


void ddmOutEstimateCountyFilter::updateVisualzation()
{
    ddmOutEstimateCountyFilterModel* model  = this->model_cast<ddmOutEstimateCountyFilterModel>();
    ddmCounty* currentCounty = model->currentCounty();

    foreach ( int county_id, model->counties() )
    {
        ddmCounty* county = model->county( county_id );
        this->drawArrow( currentCounty->countyCenter(), county->countyCenter(), 0.1 );
    }
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


void ddmOutEstimateCountyFilter::slotWidgetChangedState()
{
    ddmBaseCountyFilter::blockSignals( true );
    ddmBaseCountyFilter::slotWidgetChangedState();
    ddmBaseCountyFilter::blockSignals( false );
}


void ddmOutEstimateCountyFilter::slotWidgetChangedCounty()
{
    ddmBaseCountyFilterWidget* widget = this->widget_cast<ddmBaseCountyFilterWidget>();
    QString countyName = widget->currentCounty();
    if ( !countyName.isEmpty() )
    {
        ddmBaseCountyFilterModel*  model  = this->model_cast<ddmBaseCountyFilterModel>();
        model->blockSignals( true );
        model->setCurrentCounty( countyName );
        model->blockSignals( false );
        ddmBaseCountyFilter::saveSettings();
    }
}


ddmOutEstimateCountyFilter::~ddmOutEstimateCountyFilter()
{

}