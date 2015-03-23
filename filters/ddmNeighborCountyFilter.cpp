#include "filters/ddmNeighborCountyFilter.h"
#include "models/ddmNeighborCountyFilterModel.h"
#include "ddmMapView.h"
#include "ddmSettings.h"
#include "widgets/ddmNeighborCountyFilterWidget.h"

#include "ddmInfoLogger.h"

/**
 * Конструктор класса
 *
 * @param   parent Родитель (владелец)
 * @author  Марунин А.В.
 * @since   2.8
 */
ddmNeighborCountyFilter::ddmNeighborCountyFilter( QObject* parent ) : ddmBaseCountyFilter( parent )
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
void ddmNeighborCountyFilter::setup()
{

    ddmNeighborCountyFilterModel* model = new ddmNeighborCountyFilterModel( this );
    ddmNeighborCountyFilterWidget* widget = new ddmNeighborCountyFilterWidget( this );
    ddmInfoLogger& logger = ddmInfoLogger::instance();
    logger.writeInfo( "Выбран фильтр: Соседи 1-го порядка" );

    this->m_model = model;
    this->m_widget = widget;

    ddmBaseCountyFilter::setup();


}

/**
 * Обновляет выделенные графства
 *
 * Помечает текущее графство и выделяет его соседей
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmNeighborCountyFilter::updateSelection()
{
    ddmNeighborCountyFilterModel* model  = this->model_cast<ddmNeighborCountyFilterModel>();
    this->resetSelection();

    ddmInfoLogger& logger = ddmInfoLogger::instance();
    ddmCounty* currentCounty = model->currentCounty();
    currentCounty->show();
    currentCounty->unselect();

    foreach ( int county_id, model->neighbors() )
    {
        ddmCounty* neighbor = model->county( county_id );
        neighbor->show();
        neighbor->select();
    }

    logger.writeInfo( QObject::tr( "Отображены соседи 1-го порядка для %1" ).arg( currentCounty->geographicName() ) );
    logger.writeInfo( QObject::tr( "Количество соседей: %1" ).arg( model->neighbors().size() ) );
    Q_EMIT selectionUpdated();
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
ddmNeighborCountyFilter::~ddmNeighborCountyFilter()
{
}
