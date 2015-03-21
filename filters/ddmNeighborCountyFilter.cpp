#include "filters/ddmNeighborCountyFilter.h"
#include "models/ddmNeighborCountyFilterModel.h"
#include "ddmMapView.h"
#include "ddmSettings.h"
#include "widgets/ddmNeighborCountyFilterWidget.h"

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

    ddmCounty* currentCounty = model->currentCounty();
    currentCounty->show();
    currentCounty->unselect();

    foreach ( int county_id, model->neighbors() )
    {
        ddmCounty* neighbor = model->county( county_id );
        neighbor->show();
        neighbor->select();
    }
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
