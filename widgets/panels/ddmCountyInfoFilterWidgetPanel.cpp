#include "widgets/panels/ddmCountyInfoFilterWidgetPanel.h"
#include "ui_ddmCountyInfoFilterWidgetPanel.h"

/**
 * Конструктор класса
 *
 * @param   parent Виджет, в который встраивается панель
 * @author  Марунин А.В.
 * @since   2.8
 */
ddmCountyInfoFilterWidgetPanel::ddmCountyInfoFilterWidgetPanel( QWidget* parent ) : QWidget( parent )
{
    this->ui = new Ui::ddmCountyInfoFilterWidgetPanel;
    this->ui->setupUi( this );
}

/**
 * Задает текст панели
 *
 * @param   text Текст, отображаемый на панели
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmCountyInfoFilterWidgetPanel::setInfo( const QString& text )
{
    this->ui->countyInfo->setHtml( text );
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
ddmCountyInfoFilterWidgetPanel::~ddmCountyInfoFilterWidgetPanel()
{
    delete this->ui;
}
