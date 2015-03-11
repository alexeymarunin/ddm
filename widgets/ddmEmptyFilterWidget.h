#ifndef DDM_EMPTY_FILTER_WIDGET_H
#define DDM_EMPTY_FILTER_WIDGET_H

#include "widgets/ddmFilterWidget.h"

class ddmEmptyFilter;

namespace Ui {
    class ddmEmptyFilterWidget;
}

/**
 * Класс ddmEmptyFilterWidget описывает виджет для задания параметров фильтра ddmEmptyFilter
 *
 * Не содержит никаких элементов управления, служит исключительно как заглушка для ускорения
 * загрузки и отображения главного окна
 *
 * @author  Марунин А.В.
 * @since   2.2
 */
class ddmEmptyFilterWidget : public ddmFilterWidget
{
    Q_OBJECT

public:

    ddmEmptyFilterWidget( ddmEmptyFilter* filter, QWidget* parent = 0 );

    ~ddmEmptyFilterWidget();

private:

    Ui::ddmEmptyFilterWidget* ui;
};

#endif // DDM_EMPTY_FILTER_WIDGET_H
