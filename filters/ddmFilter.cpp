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
 * @brief ddmFilter::setup
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
 * @brief ddmFilter::updateData
 *
 * @param   fromWidget
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmFilter::updateData( bool fromWidget )
{
    Q_UNUSED( fromWidget )
}

/**
 * @brief ddmFilter::updateSelection
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmFilter::updateSelection()
{
}

/**
 * @brief ddmFilter::activate
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
 * @brief ddmFilter::deactivate
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
 * @brief ddmFilter::apply
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
 * @brief ddmFilter::resetSelection
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
 * @brief ddmFilter::selection
 *
 * @return
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
 * @brief ddmFilter::setMapCenter
 *
 * @param   center
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
 * @brief ddmFilter::setMapCenter
 *
 * @param   x
 * @param   y
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
 * @brief ddmFilter::isMapLoaded
 *
 * @return
 * @author  Марунин А.В.
 * @since   2.0
 */
bool ddmFilter::isMapLoaded() const
{
    return this->mapView()->mapReady();
}

/**
 * @brief ddmFilter::slotModelChanged
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
 * @brief ddmFilter::slotWidgetChanged
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmFilter::slotWidgetChanged()
{
    // this->apply();
}

/**
 * @brief ddmFilter::slotMapLoaded
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmFilter::slotMapLoaded()
{
}

/**
 * @brief ddmFilter::slotJavaScriptWindowObjectCleared
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmFilter::slotJavaScriptWindowObjectCleared()
{
    this->mapView()->addToJavaScriptWindowObject( "ddmFilter", this );
}

/**
 * @brief ddmFilter::valid
 *
 * @return
 * @author  Марунин А.В.
 * @since   2.0
 */
bool ddmFilter::valid() const
{
    return ( this->model() && this->widget() && this->mapView() );
}

/**
 * @brief ddmFilter::model
 *
 * @return
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmFilterModel* ddmFilter::model() const
{
    return this->m_model;
}

/**
 * @brief ddmFilter::widget
 *
 * @return
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmFilterWidget* ddmFilter::widget() const
{
    return this->m_widget;
}

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
 * @brief ddmFilter::~ddmFilter
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmFilter::~ddmFilter()
{
    this->saveSettings();
}
