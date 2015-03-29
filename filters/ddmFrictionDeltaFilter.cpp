#include "base/ddmCounty.h"

#include "filters/ddmFrictionDeltaFilter.h"
#include "models/ddmFrictionDeltaFilterModel.h"
#include "widgets/ddmFrictionDeltaFilterWidget.h"

#include "ddmSettings.h"
#include "ddmInfoLogger.h"


ddmFrictionDeltaFilter::ddmFrictionDeltaFilter( QObject* parent ) : ddmFilter( parent )
{

}

void ddmFrictionDeltaFilter::setup()
{
    ddmFrictionDeltaFilterModel* model = new ddmFrictionDeltaFilterModel( this );
    ddmFrictionDeltaFilterWidget* widget = new ddmFrictionDeltaFilterWidget( this );
    ddmInfoLogger& logger = ddmInfoLogger::instance();
    logger.writeInfo( "Выбран фильтр: Диапазон трений по центрам миграции" );


    this->m_model = model;
    this->m_widget = widget;

    // Загружаем настройки
    this->loadSettings();

    // задаем режим дельты (положительная/отрицательная)
    model->setDeltaMode( widget->deltaMode() );

    // Обязательно вызываем метод из базового класса!
    ddmFilter::setup();
}


void ddmFrictionDeltaFilter::updateData( bool fromWidget )
{
    if ( this->valid() )
    {
        ddmFrictionDeltaFilterModel* model = this->model_cast<ddmFrictionDeltaFilterModel>();
        ddmFrictionDeltaFilterWidget* widget = this->widget_cast<ddmFrictionDeltaFilterWidget>();
        if ( fromWidget )
        {
            model->setBounds( widget->minBound(), widget->maxBound() );
            model->setDeltaMode( widget->deltaMode() );
        }
        else
        {
            widget->setMinFiction( model->minFriction() );
            widget->setMaxFiction( model->maxFriction() );

            widget->setMinBound( model->minBound() );
            widget->setMaxBound( model->maxBound() );

            widget->setDeltaMode( model->deltaMode() );

            this->saveSettings();
        }
    }
}


void ddmFrictionDeltaFilter::updateSelection()
{
    ddmFilter::resetSelection();
    ddmFrictionDeltaFilterModel* model = this->model_cast<ddmFrictionDeltaFilterModel>();
    QVariantList counties = model->counties();
    ddmInfoLogger& logger = ddmInfoLogger::instance();
    QString mode = model->deltaMode() == 0 ? "положительной": "отрицательной";
    logger.writeInfo( QObject::tr( "Отображаются графства с %1 дельтой и значениями трения от %2 до %3 " ).arg( mode ).arg( model->minBound() ).arg( model->maxBound() ) );
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
 * @author  Марунин А.В.
 * @since   2.7
 */
void ddmFrictionDeltaFilter::loadSettings()
{
    ddmSettings* settings = ddmSettings::instance();

    double minFriction = settings->value( "ddmFrictionDeltaFilter/min_friction", 0.02 ).toDouble();
    double maxFriction = settings->value( "ddmFrictionDeltaFilter/max_friction", 0.04 ).toDouble();
    int deltaMode = settings->value( "ddmFrictionDeltaFilter/mode", 0 ).toInt();

    ddmFrictionDeltaFilterModel* model  = this->model_cast<ddmFrictionDeltaFilterModel>();
    model->setBounds( minFriction, maxFriction );
    model->setDeltaMode( deltaMode );
}

/**
 * Сохраняет настройки фильтра
 *
 * @author  Марунин А.В.
 * @since   2.7
 */
void ddmFrictionDeltaFilter::saveSettings()
{
    ddmSettings* settings = ddmSettings::instance();

    ddmFrictionDeltaFilterModel* model  = this->model_cast<ddmFrictionDeltaFilterModel>();
    settings->setValue( "ddmFrictionDeltaFilter/min_friction", model->minBound() );
    settings->setValue( "ddmFrictionDeltaFilter/max_friction", model->maxBound() );
    settings->setValue( "ddmFrictionDeltaFilter/mode", model->deltaMode() );
}

ddmFrictionDeltaFilter::~ddmFrictionDeltaFilter()
{
}


