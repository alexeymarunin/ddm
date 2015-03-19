#ifndef DDM_NEIGHBOR_COUNTY_FILTER_WIDGET_H
#define DDM_NEIGHBOR_COUNTY_FILTER_WIDGET_H

#include <QComboBox>
#include "widgets/ddmBaseCountyFilterWidget.h"

class ddmNeighborCountyFilter;

/**
 * Класс ddmNeighborCountyFilterWidget описывает виджет для задания параметров фильтра ddmNeighborCountyFilter
 *
 * @author  Марунин А.В.
 * @since   2.8
 * @todo    Если понадобятся доп.настройки, то нужно создать панель (см.ddmCountyInfoFilterWidgetPanel)
 */
class ddmNeighborCountyFilterWidget : public ddmBaseCountyFilterWidget
{
    Q_OBJECT

public:

    ddmNeighborCountyFilterWidget( ddmNeighborCountyFilter* filter, QWidget* parent = NULL );

    virtual ~ddmNeighborCountyFilterWidget();

private:

};

#endif // DDM_NEIGHBOR_COUNTY_FILTER_WIDGET_H
