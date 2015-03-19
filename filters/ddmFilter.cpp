#include <QtGlobal>
#include "base/ddmCounty.h"
#include "widgets/ddmCentralWidget.h"
#include "ddmMapView.h"
#include "ddmApplication.h"
#include "ddmSettings.h"
#include "filters/ddmFilter.h"


/**
 * Конструктор класса
 *
 * @param   parent Владелец фильтра
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmFilter::ddmFilter( QObject* parent ) : ddmObject( parent ),
    m_model( NULL ), m_widget( NULL )
{
    this->setObjectName( "ddmFilter" );
    this->m_mapView = new ddmMapView( this );
}

/**
 * Выполняет инициализацию фильтра при создании
 *
 * Может (и должен) перегружаться в наследуемых классах.
 * При этом настоятельно рекомендуется вызывать в конце и базовый метод.
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmFilter::setup()
{
    Q_ASSERT( this->model() != NULL );
    Q_ASSERT( this->widget() != NULL );

    this->model()->reloadData();
    this->updateData( false );

    QObject::connect( this->model(),  SIGNAL( changed() ), this, SLOT( slotModelChanged()  ) );
    QObject::connect( this->widget(), SIGNAL( changed() ), this, SLOT( slotWidgetChanged() ) );

    ddmMapView* mapView = this->mapView();
    QObject::connect( mapView, SIGNAL( javaScriptWindowObjectCleared() ), this, SLOT( slotJavaScriptWindowObjectCleared() ) );
    QObject::connect( mapView, SIGNAL( loaded() ), this, SLOT( slotMapLoaded() ) );

}

/**
 * Синхронизирует данные между виджетом и моделью
 *
 * Если параметр fromWidget = true (по умолчанию), то данные с виджета заносятся в модель
 *
 * @param   Флаг, который указывает направление синхронизации.
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmFilter::updateData( bool fromWidget )
{
    Q_UNUSED( fromWidget )
}

/**
 * Обновляет список выделенных (отображаемых) графств
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmFilter::updateSelection()
{
}

/**
 * Активирует фильтр
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmFilter::activate()
{
    bool valid = this->valid();
    if ( !valid )
    {
        this->setup();
    }

    this->widget()->show();
    this->mapView()->show();

    // При первом запуске сразу делаем автоматический Apply
    if ( !valid )
    {
        this->apply();
    }
}

/**
 * Деактивирует фильтр
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmFilter::deactivate()
{
    this->widget()->hide();
    this->mapView()->hide();
}

/**
 * Применяет фильтр
 *
 * При этом данные с виджета заносятся в модель, карта обновляется.
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmFilter::apply()
{
    if ( this->valid() )
    {
        ddmMapView* mapView = this->mapView();
        mapView->hide();
        this->updateData();
        mapView->reload();
        this->updateSelection();
        mapView->show();
    }
}

/**
 * Сбрасывает все выделенные на карте графства
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmFilter::resetSelection()
{
    QVariantList selection = this->selection();
    bool updated = false;
    foreach ( QVariant obj, selection )
    {
        ddmCounty* value = obj.value<ddmCounty*>();
        if ( value->visible() )
        {
            value->hide();
            updated = true;
        }
    }

    if ( updated )
    {
        Q_EMIT selectionUpdated();
    }
}

/**
 * Возвращает список выделенных графств
 *
 * Используется в JavaScript
 *
 * @return  Возвращаает объект типа QVariantList
 * @author  Марунин А.В.
 * @since   2.0
 */
QVariantList ddmFilter::selection() const
{
    QVariantList selection;
    QVariantList counties = this->model()->counties();
    foreach ( QVariant obj, counties )
    {
        ddmCounty* value = obj.value<ddmCounty*>();
        if ( value->visible() )
        {
            selection.append( obj );
        }
    }

    return selection;
}

/**
 * Задает центр отображаемой области карты (центрирует карту)
 *
 * @param   center Объект типа QVariantMap, который содержит координаты центра
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmFilter::setMapCenter( const QVariantMap& center )
{
    if ( this->valid() )
    {
        this->mapView()->setCenter( center );
    }
}

/**
 * Задает центр отображаемой области карты (центрирует карту)
 *
 * @param   x Координата х центра
 * @param   y Координата y центра
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmFilter::setMapCenter( double x, double y )
{
    if ( this->valid() )
    {
        this->mapView()->setCenter( x, y );
    }
}

/**
 * Определяет, полностью ли загружена карта
 *
 * @return  true если карта полностью прогрузилась
 * @author  Марунин А.В.
 * @since   2.0
 */
bool ddmFilter::isMapLoaded() const
{
    return this->mapView()->mapReady();
}

/**
 * Обработчик изменений в моделе фильтра
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmFilter::slotModelChanged()
{
    this->updateData( false );
    // this->apply();
}

/**
 * Обработчик изменений в виджете фильтра
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmFilter::slotWidgetChanged()
{
    // this->apply();
}

/**
 * Обработчик окончания загрузки карты
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmFilter::slotMapLoaded()
{
}

/**
 * Вызывается, когда карта обновляется (до slotMapLoaded)
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmFilter::slotJavaScriptWindowObjectCleared()
{
    this->mapView()->addToJavaScriptWindowObject( "ddmFilter", this );
}

/**
 * Проверяет валидность фильтра
 *
 * Проверяет, созданы ли модель, виджет и карта.
 *
 * @return  true если фильтр полностью готов к работе
 * @author  Марунин А.В.
 * @since   2.0
 */
bool ddmFilter::valid() const
{
    return ( this->model() && this->widget() && this->mapView() );
}

/**
 * Возвращает модель
 *
 * @return  Указатель на объект типа ddmFilterModel
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmFilterModel* ddmFilter::model() const
{
    return this->m_model;
}

/**
 * Возвращает виджет
 *
 * @return  Указатель на объект типа ddmFilterWidget
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmFilterWidget* ddmFilter::widget() const
{
    return this->m_widget;
}

/**
 * Возвращает карту
 *
 * @return  Указатель на объект типа ddmMapView
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmMapView* ddmFilter::mapView() const
{
    return this->m_mapView;
}

/**
 * Загружает настройки фильтра
 *
 * По умолчанию нигде не вызывается.
 * Разработчик фильтра самостоятельно должен решить, когда нужно загрузить настройки:
 * в конструкторе, в методе setup() и т.д.
 * Наиболее логичное использование - задать те значения виджета и/или модели,
 * которые использовались при последнем запуске приложения.
 *
 * @author  Марунин А.В.
 * @since   2.6
 */
void ddmFilter::loadSettings()
{
    ddmSettings* settings = ddmSettings::instance();
    Q_UNUSED( settings );
}

/**
 * Сохраняет настройки фильтра
 *
 * По умолчанию вызывается в деструкторе фильтра.
 * Рекомендуется сохранять настройки в отдельную секцию,
 * имя которой совпадает с именем класса фильтра, например:
 *      ddmCountyFilter/county
 *      ddmFrictionDeltaFilter/bounds
 * и т.д.
 *
 * @author  Марунин А.В.
 * @since   2.6
 */
void ddmFilter::saveSettings()
{
    ddmSettings* settings = ddmSettings::instance();
    Q_UNUSED( settings );
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmFilter::~ddmFilter()
{
    this->saveSettings();
}
