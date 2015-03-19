#include "widgets/ddmNeighborCountyFilterWidget.h"
#include "filters/ddmNeighborCountyFilter.h"
#include "ui_ddmNeighborCountyFilterWidget.h"

/**
 * Конструктор класса
 *
 * @param   filter Фильтр, которому принадлежит виджет
 * @param   parent Родитель (владелец) виджета
 * @author  Марунин А.В.
 * @since   2.8
 */
ddmNeighborCountyFilterWidget::ddmNeighborCountyFilterWidget( ddmNeighborCountyFilter* filter, QWidget* parent ) : ddmBaseCountyFilterWidget( filter, parent )
{
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
ddmNeighborCountyFilterWidget::~ddmNeighborCountyFilterWidget()
{
}
