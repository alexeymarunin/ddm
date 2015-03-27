#include "filters/ddmBaseCountyFilter.h"
#include "models/ddmBaseCountyFilterModel.h"
#include "ddmMapView.h"
#include "ddmSettings.h"
#include "widgets/ddmBaseCountyFilterWidget.h"

/**
 * Конструктор класса
 *
 * @param   parent Родитель (владелец)
 * @author  Марунин А.В.
 * @since   2.8
 */
ddmBaseCountyFilter::ddmBaseCountyFilter( QObject* parent ) : ddmFilter( parent )
{
}

/**
 * Создает фильтр
 *
 * Создает модель фильтра и виджет, настраивает сигнал-слоты
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmBaseCountyFilter::setup()
{
    // Модель и виджет должны быть созданы в дочернем setup()
    Q_ASSERT( this->model() != NULL );
    Q_ASSERT( this->widget() != NULL );

    // Загружаем предыдущие значения
    this->loadSettings();

    ddmBaseCountyFilterModel*  model  = this->model_cast<ddmBaseCountyFilterModel>();
    ddmBaseCountyFilterWidget* widget = this->widget_cast<ddmBaseCountyFilterWidget>();

    widget->setStateNames( model->stateNames() );
    widget->setCurrentState( model->currentState()->geographicName() );
    widget->setCountyNames( model->currentState()->countyNames() );
    widget->setCurrentCounty( model->currentCounty()->geographicName() );

    QObject::connect( widget, SIGNAL( changedState()  ), this, SLOT( slotWidgetChangedState()  ), Qt::UniqueConnection );
    QObject::connect( widget, SIGNAL( changedCounty() ), this, SLOT( slotWidgetChangedCounty() ), Qt::UniqueConnection );

    ddmFilter::setup();

}

/**
 * Синхронизирует данные между моделью и виджетом
 *
 * @param   fromWidget Флаг, указывающий направление синхронизации:
 *              true - обновить данные модели из значений элементов управления виджета
 *              false - обновить элементы управления на виджете по данным модели
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmBaseCountyFilter::updateData( bool fromWidget )
{
    if ( this->valid() )
    {
        ddmBaseCountyFilterModel*  model  = this->model_cast<ddmBaseCountyFilterModel>();
        ddmBaseCountyFilterWidget* widget = this->widget_cast<ddmBaseCountyFilterWidget>();
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

/**
 * Обработчик события изменения текущего элемента в выпадающем списке штатов
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmBaseCountyFilter::slotWidgetChangedState()
{
    ddmBaseCountyFilterWidget* widget = this->widget_cast<ddmBaseCountyFilterWidget>();
    QString stateName = widget->currentState();
    if ( !stateName.isEmpty() )
    {
        this->updateData();
        ddmBaseCountyFilterModel* model = qobject_cast<ddmBaseCountyFilterModel*>( this->model() );
        QStringList countyNames = model->currentState()->countyNames();
        widget->setCountyNames( countyNames );
    }
}

/**
 * Обработчик события изменения текущего элемента в выпадающем списке графств
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmBaseCountyFilter::slotWidgetChangedCounty()
{
    ddmBaseCountyFilterWidget* widget = this->widget_cast<ddmBaseCountyFilterWidget>();
    QString countyName = widget->currentCounty();
    if ( !countyName.isEmpty() )
    {
        ddmBaseCountyFilterModel*  model  = this->model_cast<ddmBaseCountyFilterModel>();
        model->setCurrentCounty( countyName );
        this->apply();

        this->fitSelection();

        this->saveSettings();
    }
}

/**
 * Центрирует текущее выделение
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmBaseCountyFilter::fitSelection()
{
    this->mapView()->fitSelection();
}

/**
 * Обработчик, вызываемый после обновления карты
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmBaseCountyFilter::slotMapLoaded()
{
    this->updateData();
    this->fitSelection();
}

/**
 * Загружает настройки фильтра
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmBaseCountyFilter::loadSettings()
{
    ddmSettings* settings = ddmSettings::instance();

    int lastCounty = settings->value( QString( this->metaObject()->className() ) + "/county", 0 ).toInt();
    if ( lastCounty > 0 )
    {
        ddmBaseCountyFilterModel* model  = this->model_cast<ddmBaseCountyFilterModel>();
        model->setCurrentCounty( lastCounty );
    }
}

/**
 * Сохраняет настройки фильтра
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmBaseCountyFilter::saveSettings()
{
    ddmSettings* settings = ddmSettings::instance();

    ddmBaseCountyFilterModel* model  = this->model_cast<ddmBaseCountyFilterModel>();
    settings->setValue( QString( this->metaObject()->className() ) + "/county", model->currentCounty()->id() );
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
ddmBaseCountyFilter::~ddmBaseCountyFilter()
{
}
