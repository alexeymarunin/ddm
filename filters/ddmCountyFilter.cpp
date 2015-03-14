#include "filters/ddmCountyFilter.h"
#include "models/ddmCountyFilterModel.h"
#include "ddmMapView.h"
#include "ddmSettings.h"
#include "widgets/ddmCountyFilterWidget.h"

/**
 * Конструктор класса
 *
 * @param   parent Родитель (владелец)
 * @author  Марунин А.В.
 * @since   2.1
 */
ddmCountyFilter::ddmCountyFilter( QObject* parent ) : ddmFilter( parent )
{
}

/**
 * Создает фильтр
 *
 * Создает модель фильтра и виджет, настраивает сигнал-слоты
 *
 * @author  Марунин А.В.
 * @since   2.1
 */
void ddmCountyFilter::setup()
{

    ddmCountyFilterModel* model = new ddmCountyFilterModel( this );
    ddmCountyFilterWidget* widget = new ddmCountyFilterWidget( this );

    this->m_model = model;
    this->m_widget = widget;

    // Загружаем предыдущие значения
    this->loadSettings();
    qDebug() << model->currentCounty()->geographicName();

    widget->setStateNames( model->stateNames() );
    widget->setCurrentState( model->currentState()->geographicName() );
    widget->setCountyNames( model->currentState()->countyNames() );
    widget->setCurrentCounty( model->currentCounty()->geographicName() );

    QObject::connect( widget, SIGNAL( changedState()  ), this, SLOT( slotWidgetChangedState()  ) );
    QObject::connect( widget, SIGNAL( changedCounty() ), this, SLOT( slotWidgetChangedCounty() ) );

    // Обязательно вызываем метод из базового класса!
    ddmFilter::setup();

}

/**
 * Синхронизирует данные между моделью и виджетом
 *
 * @param   fromWidget Флаг, указывающий направление синхронизации:
 *              true - обновить данные модели из значений элементов управления виджета
 *              false - обновить элементы управления на виджете по данным модели
 * @author  Марунин А.В.
 * @since   2.1
 */
void ddmCountyFilter::updateData( bool fromWidget )
{
    if ( this->valid() )
    {
        ddmCountyFilterModel*  model  = this->model_cast<ddmCountyFilterModel>();
        ddmCountyFilterWidget* widget = this->widget_cast<ddmCountyFilterWidget>();
        if ( fromWidget )
        {
            if ( model->currentState() && model->currentState()->geographicName() != widget->currentState() )
            {
                model->setCurrentState( widget->currentState() );
            }
            else if ( model->currentCounty() && model->currentCounty()->geographicName() != widget->currentCounty() )
            {
                model->setCurrentCounty( widget->currentCounty() );
            }
        }
        else
        {
            if ( model->currentState() )
            {
                widget->setCurrentState( model->currentState()->geographicName() );
            }
            if ( model->currentCounty() )
            {
                widget->setCurrentCounty( model->currentCounty()->geographicName() );
            }
        }
    }
}

void ddmCountyFilter::updateSelection()
{
    ddmCountyFilterModel* model  = this->model_cast<ddmCountyFilterModel>();
    this->resetSelection();
    model->currentCounty()->show();
    Q_EMIT selectionUpdated();
}

void ddmCountyFilter::slotWidgetChangedState()
{
    ddmCountyFilterWidget* widget = this->widget_cast<ddmCountyFilterWidget>();
    QString stateName = widget->currentState();
    if ( !stateName.isEmpty() )
    {
        this->updateData();
        ddmCountyFilterModel* model = qobject_cast<ddmCountyFilterModel*>( this->model() );
        QStringList countyNames = model->currentState()->countyNames();
        widget->setCountyNames( countyNames );
    }
}

void ddmCountyFilter::slotWidgetChangedCounty()
{
    ddmCountyFilterWidget* widget = this->widget_cast<ddmCountyFilterWidget>();
    QString countyName = widget->currentCounty();
    if ( !countyName.isEmpty() )
    {
        ddmCountyFilterModel*  model  = this->model_cast<ddmCountyFilterModel>();
        model->setCurrentCounty( countyName );
        this->apply();

        QVariantMap center = model->currentCounty()->center();
        this->setMapCenter( center );
        this->mapView()->setMarker( center );

        this->saveSettings();
    }
}

void ddmCountyFilter::slotMapLoaded()
{
    this->updateData();

    ddmCountyFilterModel*  model  = this->model_cast<ddmCountyFilterModel>();
    QVariantMap center = model->currentCounty()->center();
    this->setMapCenter( center );

    this->mapView()->setMarker( center );
}

/**
 * Загружает настройки фильтра
 *
 * @author  Марунин А.В.
 * @since   2.6
 */
void ddmCountyFilter::loadSettings()
{
    ddmSettings* settings = ddmSettings::instance();

    int lastCounty = settings->value( "ddmCountyFilter/county", 0 ).toInt();
    if ( lastCounty > 0 )
    {
        ddmCountyFilterModel* model  = this->model_cast<ddmCountyFilterModel>();
        model->setCurrentCounty( lastCounty );
    }
}

/**
 * Сохраняет настройки фильтра
 *
 * @author  Марунин А.В.
 * @since   2.6
 */
void ddmCountyFilter::saveSettings()
{
    ddmSettings* settings = ddmSettings::instance();

    ddmCountyFilterModel* model  = this->model_cast<ddmCountyFilterModel>();
    settings->setValue( "ddmCountyFilter/county", model->currentCounty()->id() );
}

/**
 * @brief ddmCountyFilter::~ddmCountyFilter
 */
ddmCountyFilter::~ddmCountyFilter()
{
}
