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

    widget->setStateNames( model->stateNames() );
    widget->setCurrentState( model->currentState()->geographicName() );
    widget->setCountyNames( model->currentState()->countyNames() );
    widget->setCurrentCounty( model->currentCounty()->geographicName() );

    QObject::connect( widget, SIGNAL( changedState()  ), this, SLOT( slotWidgetChangedState()  ) );
    QObject::connect( widget, SIGNAL( changedCounty() ), this, SLOT( slotWidgetChangedCounty() ) );

    // Обязательно вызываем метод из базового класса!
    ddmFilter::setup();

    this->updateInfo();

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

        this->updateInfo();

        QVariantMap center = model->currentCounty()->center();
        this->setMapCenter( center );
        this->mapView()->setMarker( center );

        this->saveSettings();
    }
}

/**
 * Обновляет на виджете информацию о текущем графстве
 *
 * @author  Марунин А.В.
 * @since   2.6
 */
void ddmCountyFilter::updateInfo()
{
    ddmCountyFilterModel* model  = this->model_cast<ddmCountyFilterModel>();
    ddmCounty* county = model->currentCounty();
    QString countyInfoHTML;
    if ( county )
    {
        countyInfoHTML = QString(
            "<table>"
            "<tr>"
                "<td><b>Название: </b></td>"
                "<td>%1</td>"
            "</tr>"
            "<tr>"
                "<td><b>Штат: </b></td>"
                "<td>%2</td>"
            "</tr>"
            "<tr>"
                "<td><b>Население: </b></td>"
                "<td>%3 чел.</td>"
            "</tr>"
            "<tr>"
                "<td><b>Приехавших: </b></td>"
                "<td>%4 чел.</td>"
            "</tr>"
            "<tr>"
                "<td><b>Уехавших: </b></td>"
                "<td>%5 чел.</td>"
            "</tr>"
            "<tr>"
                "<td><b>Дельта: </b></td>"
                "<td>%6 чел.</td>"
            "</tr>"
            "<tr>"
                "<td><b>Трение приехав.: </b></td>"
                "<td>%7</td>"
            "</tr>"
            "<tr>"
                "<td><b>Трение уехав.: </b></td>"
                "<td>%8</td>"
            "</tr>"
            "<tr>"
                "<td><b>Трение средн.: </b></td>"
                "<td>%9</td>"
            "</tr>"
            "</table>"
            )
            .arg( county->geographicName() )
            .arg( county->state()->geographicName() )
            .arg( county->population() )
            .arg( county->in_sum() )
            .arg( county->out_sum() )
            .arg( county->delta() )
            .arg( county->f_in_mid() )
            .arg( county->f_out_mid() )
            .arg( county->f_mid() );
    }

    ddmCountyFilterWidget* widget = this->widget_cast<ddmCountyFilterWidget>();
    widget->setCountyInfo( countyInfoHTML );
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
