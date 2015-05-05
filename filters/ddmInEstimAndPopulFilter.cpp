#include "filters/ddmInEstimAndPopulFilter.h"
#include "models/ddmInEstimAndPopulFilterModel.h"
#include "widgets/ddmInEstimAndPopulFilterWidget.h"

#include "ddmInfoLogger.h"
#include "ddmSettings.h"



ddmInEstimAndPopulFilter::ddmInEstimAndPopulFilter( QObject *parent ):
    ddmBaseCountyFilter( parent )
{

}


void ddmInEstimAndPopulFilter::setup()
{
    ddmInEstimAndPopulFilterModel* model = new ddmInEstimAndPopulFilterModel( this );
    ddmInEstimAndPopulFilterWidget* widget = new ddmInEstimAndPopulFilterWidget( this );
    ddmInfoLogger& logger = ddmInfoLogger::instance();
    logger.writeInfo( "Выбран фильтр: Миграция в графство относительно размеров графства" );

    this->m_model = model;
    this->m_widget = widget;

    this->loadSettings();

    ddmBaseCountyFilter::setup();

    QObject::connect( widget, SIGNAL( changedState()  ), this, SLOT( slotWidgetChangedState()  ), Qt::UniqueConnection );
    QObject::connect( widget, SIGNAL( changedCounty() ), this, SLOT( slotWidgetChangedCounty() ), Qt::UniqueConnection );

}

void ddmInEstimAndPopulFilter::updateData( bool fromWidget )
{
    if ( this->valid() )
    {
        ddmInEstimAndPopulFilterModel*  model  = this->model_cast<ddmInEstimAndPopulFilterModel>();
        ddmInEstimAndPopulFilterWidget* widget = this->widget_cast<ddmInEstimAndPopulFilterWidget>();
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


void ddmInEstimAndPopulFilter::updateSelection()
{
    ddmInEstimAndPopulFilterModel* model  = this->model_cast<ddmInEstimAndPopulFilterModel>();
    this->resetSelection();
    qlonglong population = model->getCurentCountyPopulation();
    double mult = 0.01;
    double minPopBound = ( model->minPopBound() * mult );
    double maxPopBound = ( model->maxPopBound() * mult );

    ddmInfoLogger& logger = ddmInfoLogger::instance();
    logger.writeInfo( QObject::tr( "Численость графства: %1 человек" ).arg( population ) );
    logger.writeInfo( QObject::tr( "Отобржаются графства из которых приехало от: %2% ( %3 человек(а) ) до %4% ( %5 человек(а) ) процентов от населения выбранного графства" )
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


void ddmInEstimAndPopulFilter::saveSettings()
{
    ddmSettings* settings = ddmSettings::instance();
    ddmBaseCountyFilter::saveSettings();
    ddmInEstimAndPopulFilterModel* model  = this->model_cast<ddmInEstimAndPopulFilterModel>();
    settings->setValue( "ddmInEstimAndPopulFilter/min_population", model->minPopBound() );
    settings->setValue( "ddmInEstimAndPopulFilter/max_population", model->maxPopBound() );
}


void ddmInEstimAndPopulFilter::loadSettings()
{
    ddmSettings* settings = ddmSettings::instance();
    ddmBaseCountyFilter::loadSettings();
    ddmInEstimAndPopulFilterModel* model  = this->model_cast<ddmInEstimAndPopulFilterModel>();
    double minBound = settings->value( "ddmInEstimAndPopulFilter/min_population", 0.01 ).toDouble();
    double maxBound = settings->value( "ddmInEstimAndPopulFilter/max_population", 0.1 ).toDouble();
    model->setPopBounds( minBound, maxBound );
}


void ddmInEstimAndPopulFilter::slotWidgetChangedState()
{
    ddmBaseCountyFilter::blockSignals( true );
    ddmBaseCountyFilter::slotWidgetChangedState();
    ddmBaseCountyFilter::blockSignals( false );
}


void ddmInEstimAndPopulFilter::slotWidgetChangedCounty()
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


void ddmInEstimAndPopulFilter::slotUpdateVisualzation()
{
    ddmInEstimAndPopulFilterModel* model  = this->model_cast<ddmInEstimAndPopulFilterModel>();
    ddmCounty* currentCounty = model->currentCounty();

    if ( !currentCounty->visible() )
        return;

    foreach ( int county_id, model->counties() )
    {
        ddmCounty* county = model->county( county_id );
        this->drawArrow( county->countyCenter(), currentCounty->countyCenter(), 0.1 );
    }
}


ddmInEstimAndPopulFilter::~ddmInEstimAndPopulFilter()
{

}
