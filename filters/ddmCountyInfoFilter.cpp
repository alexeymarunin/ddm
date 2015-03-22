#include "ddmMapView.h"
#include "ddmSettings.h"
#include "ddmInfoLogger.h"

#include "models/ddmCountyInfoFilterModel.h"
#include "widgets/ddmCountyInfoFilterWidget.h"
#include "filters/ddmCountyInfoFilter.h"

/**
 * Конструктор класса
 *
 * @param   parent Родитель (владелец)
 * @author  Марунин А.В.
 * @since   2.8
 */
ddmCountyInfoFilter::ddmCountyInfoFilter( QObject* parent ) : ddmBaseCountyFilter( parent )
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
void ddmCountyInfoFilter::setup()
{
    ddmInfoLogger& logger = ddmInfoLogger::instance();
    logger.writeInfo( "Выбран фильтр информации по графствам..." );

    ddmCountyInfoFilterModel* model = new ddmCountyInfoFilterModel( this );
    ddmCountyInfoFilterWidget* widget = new ddmCountyInfoFilterWidget( this );

    this->m_model = model;
    this->m_widget = widget;

    // Обязательно вызываем метод из базового класса!
    ddmBaseCountyFilter::setup();

    this->updateInfo();

}

/**
 * Обновляет список выделенных (отображаемых) графств
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmCountyInfoFilter::updateSelection()
{
    ddmCountyInfoFilterModel* model  = this->model_cast<ddmCountyInfoFilterModel>();
    ddmInfoLogger& logger = ddmInfoLogger::instance();
    this->resetSelection();

    model->currentCounty()->show();
    Q_EMIT selectionUpdated();
    logger.writeInfo( QObject::tr( "Отображено графство: %1" ).arg( model->currentCounty()->geographicName() ) );
}

/**
 * Обработчик события изменения текущего элемента в выпадающем списке графств
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmCountyInfoFilter::slotWidgetChangedCounty()
{
    ddmBaseCountyFilter::slotWidgetChangedCounty();
    this->updateInfo();
}

/**
 * Обновляет на виджете информацию о текущем графстве
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmCountyInfoFilter::updateInfo()
{
    ddmCountyInfoFilterModel* model  = this->model_cast<ddmCountyInfoFilterModel>();
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

    ddmCountyInfoFilterWidget* widget = this->widget_cast<ddmCountyInfoFilterWidget>();
    widget->setInfo( countyInfoHTML );
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
ddmCountyInfoFilter::~ddmCountyInfoFilter()
{
}
