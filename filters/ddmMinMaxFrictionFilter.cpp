#include "base/ddmCounty.h"
#include "models/ddmMinMaxFrictionFilterModel.h"
#include "widgets/ddmMinMaxFrictionFilterWidget.h"
#include "filters/ddmMinMaxFrictionFilter.h"
#include "ddmSettings.h"


/**
 * @brief ddmMinMaxFrictionFilter::ddmMinMaxFrictionFilter
 * @param parent
 * @author  Марунин А.В.
 * @since   2.3
 */
ddmMinMaxFrictionFilter::ddmMinMaxFrictionFilter( QObject* parent ) : ddmFilter( parent )
{
}

/**
 * Выполняет инициализацию фильтра при создании
 *
 * @author  Марунин А.В.
 * @since   2.3
 */
void ddmMinMaxFrictionFilter::setup()
{
    ddmMinMaxFrictionFilterModel* model = new ddmMinMaxFrictionFilterModel( this );
    ddmMinMaxFrictionFilterWidget* widget = new ddmMinMaxFrictionFilterWidget( this );

    this->m_model = model;
    this->m_widget = widget;

    // Загружаем настройки
    this->loadSettings();

    // Обязательно вызываем метод из базового класса!
    ddmFilter::setup();

}

/**
 * Синхронизирует данные между виджетом и моделью
 *
 * Если параметр fromWidget = true (по умолчанию), то данные с виджета заносятся в модель
 *
 * @param   Флаг, который указывает направление синхронизации.
 * @author  Марунин А.В.
 * @since   2.3
 */
void ddmMinMaxFrictionFilter::updateData( bool fromWidget )
{
    if ( this->valid() )
    {
        ddmMinMaxFrictionFilterModel* model = this->model_cast<ddmMinMaxFrictionFilterModel>();
        ddmMinMaxFrictionFilterWidget* widget = this->widget_cast<ddmMinMaxFrictionFilterWidget>();
        if ( fromWidget )
        {
            model->setBounds( widget->minBound(), widget->maxBound() );
        }
        else
        {
            widget->setMinFiction( model->minFriction() );
            widget->setMaxFiction( model->maxFriction() );

            widget->setMinBound( model->minBound() );
            widget->setMaxBound( model->maxBound() );

            this->saveSettings();
        }
    }
}

/**
 * Обновляет выделенные графства
 *
 * @author  Марунин А.В.
 * @since   2.3
 */
void ddmMinMaxFrictionFilter::updateSelection()
{
    ddmMinMaxFrictionFilterModel* model = this->model_cast<ddmMinMaxFrictionFilterModel>();
    QVariantList counties = model->counties();
    double minBound = model->minBound();
    double maxBound = model->maxBound();

    bool needUpdate = false;
    foreach ( QVariant obj, counties )
    {
        ddmCounty* county = obj.value<ddmCounty*>();
        double f_mid = county->f_mid();
        if ( f_mid >= minBound && f_mid <= maxBound )
        {
            if ( !county->visible() )
            {
                county->show();
                needUpdate = true;
            }
            // TODO: здесь должны быть цветовые настройки
        }
        else
        {
            if ( county->visible() )
            {
                county->hide();
                needUpdate = true;
            }
        }
    }

    if ( needUpdate )
    {
        Q_EMIT selectionUpdated();
    }
}

/**
 * Загружает настройки фильтра
 *
 * @author  Марунин А.В.
 * @since   2.7
 */
void ddmMinMaxFrictionFilter::loadSettings()
{
    ddmSettings* settings = ddmSettings::instance();

    double minFriction = settings->value( "ddmMinMaxFrictionFilter/min_friction", 0.02 ).toDouble();
    double maxFriction = settings->value( "ddmMinMaxFrictionFilter/max_friction", 0.04 ).toDouble();

    ddmMinMaxFrictionFilterModel* model  = this->model_cast<ddmMinMaxFrictionFilterModel>();
    model->setBounds( minFriction, maxFriction );
}

/**
 * Сохраняет настройки фильтра
 *
 * @author  Марунин А.В.
 * @since   2.7
 */
void ddmMinMaxFrictionFilter::saveSettings()
{
    ddmSettings* settings = ddmSettings::instance();

    ddmMinMaxFrictionFilterModel* model  = this->model_cast<ddmMinMaxFrictionFilterModel>();
    settings->setValue( "ddmMinMaxFrictionFilter/min_friction", model->minBound() );
    settings->setValue( "ddmMinMaxFrictionFilter/max_friction", model->maxBound() );
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.3
 */
ddmMinMaxFrictionFilter::~ddmMinMaxFrictionFilter()
{
}


