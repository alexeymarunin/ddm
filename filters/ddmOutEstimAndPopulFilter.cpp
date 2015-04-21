#include "filters/ddmOutEstimAndPopulFilter.h"
#include "models/ddmOutEstimAndPopulFilterModel.h"
#include "widgets/ddmOutEstimAndPopulFilterWidget.h"

#include "ddmInfoLogger.h"
#include "ddmSettings.h"



ddmOutEstimAndPopulFilter::ddmOutEstimAndPopulFilter( QObject *parent ):
    ddmBaseCountyFilter( parent )
{

}


void ddmOutEstimAndPopulFilter::setup()
{
    ddmOutEstimAndPopulFilterModel* model = new ddmOutEstimAndPopulFilterModel( this );
    ddmOutEstimAndPopulFilterWidget* widget = new ddmOutEstimAndPopulFilterWidget( this );
    ddmInfoLogger& logger = ddmInfoLogger::instance();
    logger.writeInfo( "Выбран фильтр: Миграция из графства относительно размеров графства" );

    this->m_model = model;
    this->m_widget = widget;

    this->loadSettings();

    ddmBaseCountyFilter::setup();

    QObject::connect( widget, SIGNAL( changedState()  ), this, SLOT( slotWidgetChangedState()  ), Qt::UniqueConnection );
    QObject::connect( widget, SIGNAL( changedCounty() ), this, SLOT( slotWidgetChangedCounty() ), Qt::UniqueConnection );

}

void ddmOutEstimAndPopulFilter::updateData( bool fromWidget )
{
    if ( this->valid() )
    {
        ddmOutEstimAndPopulFilterModel*  model  = this->model_cast<ddmOutEstimAndPopulFilterModel>();
        ddmOutEstimAndPopulFilterWidget* widget = this->widget_cast<ddmOutEstimAndPopulFilterWidget>();
        if ( fromWidget )
        {
            model->setPopBounds( widget->minBound(), widget->maxBound() );
        }
        else
        {
            widget->setMinBound( model->minPopBound() );
            widget->setMaxBound( model->maxPopBound() );

            this->saveSettings();        }
    }
    ddmBaseCountyFilter::updateData( fromWidget );
}


void ddmOutEstimAndPopulFilter::updateSelection()
{
    ddmOutEstimAndPopulFilterModel* model  = this->model_cast<ddmOutEstimAndPopulFilterModel>();
    this->resetSelection();
    qlonglong population = model->getCurentCountyPopulation();
    double mult = 0.01;
    double minPopBound = ( model->minPopBound() * mult );
    double maxPopBound = ( model->maxPopBound() * mult );

    ddmInfoLogger& logger = ddmInfoLogger::instance();
    logger.writeInfo( QObject::tr( "Численость графства: %1 человек" ).arg( population ) );
    logger.writeInfo( QObject::tr( "Отобржаются графства в которые уехало от: %2% ( %3 человек(а) ) до %4% ( %5 человек(а) ) процентов от населения выбранного графства" )
                      .arg( model->minPopBound() ).arg( qlonglong ( population * minPopBound ) ).arg( model->maxPopBound() )
                      .arg( qlonglong ( population * maxPopBound ) ) );

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
    logger.writeInfo( QObject::tr( "Отображено %1 графств" ).arg( model->counties().size() ) );
    Q_EMIT selectionUpdated();
}


void ddmOutEstimAndPopulFilter::saveSettings()
{
    ddmSettings* settings = ddmSettings::instance();
    ddmBaseCountyFilter::saveSettings();
    ddmOutEstimAndPopulFilterModel* model  = this->model_cast<ddmOutEstimAndPopulFilterModel>();
    settings->setValue( "ddmOutEstimAndPopulFilter/min_population", model->minPopBound() );
    settings->setValue( "ddmOutEstimAndPopulFilter/max_population", model->maxPopBound() );
}


void ddmOutEstimAndPopulFilter::loadSettings()
{
    ddmSettings* settings = ddmSettings::instance();
    ddmBaseCountyFilter::loadSettings();
    ddmOutEstimAndPopulFilterModel* model  = this->model_cast<ddmOutEstimAndPopulFilterModel>();
    double minBound = settings->value( "ddmOutEstimAndPopulFilter/min_population", 0.01 ).toDouble();
    double maxBound = settings->value( "ddmOutEstimAndPopulFilter/max_population", 0.1 ).toDouble();
    model->setPopBounds( minBound, maxBound );
}


void ddmOutEstimAndPopulFilter::slotWidgetChangedState()
{
    ddmBaseCountyFilter::blockSignals( true );
    ddmBaseCountyFilter::slotWidgetChangedState();
    ddmBaseCountyFilter::blockSignals( false );
}


void ddmOutEstimAndPopulFilter::slotWidgetChangedCounty()
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


ddmOutEstimAndPopulFilter::~ddmOutEstimAndPopulFilter()
{

}
