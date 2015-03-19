#ifndef DDM_COUNTY_INFO_FILTER_WIDGET_H
#define DDM_COUNTY_INFO_FILTER_WIDGET_H

#include "widgets/ddmBaseCountyFilterWidget.h"

class ddmCountyInfoFilter;
class ddmCountyInfoFilterWidgetPanel;

/**
 * Класс ddmCountyInfoFilterWidget описывает виджет для фильтра ddmCountyInfoFilter
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
class ddmCountyInfoFilterWidget : public ddmBaseCountyFilterWidget
{
    Q_OBJECT

public:

    ddmCountyInfoFilterWidget( ddmCountyInfoFilter* filter, QWidget* parent = NULL );

    ddmCountyInfoFilterWidgetPanel* infoPanel() const;
    void setInfo( const QString& text );

    virtual ~ddmCountyInfoFilterWidget();

protected:

    ddmCountyInfoFilterWidgetPanel* m_infoPanel;
};

#endif // DDM_COUNTY_INFO_FILTER_WIDGET_H
