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

    QObject::connect( widget, SIGNAL( changedState()  ), this, SLOT( slotWidgetChangedState()  ), Qt::UniqueConnection );
    QObject::connect( widget, SIGNAL( changedCounty() ), this, SLOT( slotWidgetChangedCounty() ), Qt::UniqueConnection );
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
    logger.writeInfo( QObject::tr( "Отображаются графства, из которых приехало от %1 до %2 человек" )
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

    this->fitSelection();
    logger.writeInfo( QObject::tr( "Найдено %1 графств" ).arg( model->counties().size() ) );
    Q_EMIT selectionUpdated();
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


void ddmInEstimateCountyFilter::slotWidgetChangedState()
{
    ddmBaseCountyFilter::blockSignals( true );
    ddmBaseCountyFilter::slotWidgetChangedState();
    ddmBaseCountyFilter::blockSignals( false );
}


void ddmInEstimateCountyFilter::slotWidgetChangedCounty()
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


void ddmInEstimateCountyFilter::slotUpdateVisualzation()
{
    ddmInEstimateCountyFilterModel* model  = this->model_cast<ddmInEstimateCountyFilterModel>();
    ddmCounty* currentCounty = model->currentCounty();

    if ( !currentCounty->visible() )
        return;

    foreach ( int county_id, model->counties() )
    {
        ddmCounty* county = model->county( county_id );
        this->drawArrow( county->countyCenter(), currentCounty->countyCenter(), 0.1 );
    }

}


ddmInEstimateCountyFilter::~ddmInEstimateCountyFilter()
{

}



