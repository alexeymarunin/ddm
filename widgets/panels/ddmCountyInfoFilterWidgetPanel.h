#ifndef DDM_COUNTY_INFO_FILTER_WIDGET_PANEL_H
#define DDM_COUNTY_INFO_FILTER_WIDGET_PANEL_H

#include <QWidget>

namespace Ui {
    class ddmCountyInfoFilterWidgetPanel;
}

/**
 * Класс ddmCountyInfoFilterWidgetPanel описывает панель, отображающую информацию о выбранном графстве
 * в виджете ddmCountyInfoFilterWidget (фильтр ddmCountyInfoFilter)
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
class ddmCountyInfoFilterWidgetPanel : public QWidget
{
    Q_OBJECT

public:

    ddmCountyInfoFilterWidgetPanel(  QWidget* parent = NULL );

    void setInfo( const QString& text );

    virtual ~ddmCountyInfoFilterWidgetPanel();

private:

    Ui::ddmCountyInfoFilterWidgetPanel* ui;

};

#endif // DDM_COUNTY_INFO_FILTER_WIDGET_PANEL_H
