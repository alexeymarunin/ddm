#include "widgets/panels/ddmCountyInfoFilterWidgetPanel.h"
#include "widgets/ddmCountyInfoFilterWidget.h"
#include "filters/ddmCountyInfoFilter.h"

/**
 * Констуктор класса
 *
 * @param   filter Фильтр, к которому относится виджет
 * @param   parent Родитель (владелец) виджета
 * @author  Марунин А.В.
 * @since   2.8
 */
ddmCountyInfoFilterWidget::ddmCountyInfoFilterWidget( ddmCountyInfoFilter* filter, QWidget* parent ) : ddmBaseCountyFilterWidget( filter, parent )
{
    this->m_infoPanel = new ddmCountyInfoFilterWidgetPanel( this );

    this->insertWidget( this->infoPanel() );
}

/**
 * Доступ к информационной панели
 *
 * @return  Указатель на объект типа ddmCountyInfoFilterWidgetPanel
 * @author  Марунин А.В.
 * @since   2.8
 */
ddmCountyInfoFilterWidgetPanel* ddmCountyInfoFilterWidget::infoPanel() const
{
    return const_cast<ddmCountyInfoFilterWidget*>( this )->m_infoPanel;
}

/**
 * Задает текст панели
 *
 * @param   text Текст, отображаемый на панели
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmCountyInfoFilterWidget::setInfo( const QString& text )
{
    this->infoPanel()->setInfo( text );
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
ddmCountyInfoFilterWidget::~ddmCountyInfoFilterWidget()
{
}
