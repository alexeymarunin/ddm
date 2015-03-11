#ifndef DDM_MIN_MAX_FRICTION_FILTER_WIDGET_H
#define DDM_MIN_MAX_FRICTION_FILTER_WIDGET_H

#include <QDoubleSpinBox>
#include "widgets/ddmFilterWidget.h"

class ddmMinMaxFrictionFilter;

namespace Ui {
    class ddmMinMaxFrictionFilterWidget;
}

/**
 * Класс ddmMinMaxFrictionFilterWidget описывает виджет для задания параметров фильтра ddmMinMaxFrictionFilter
 *
 * Содержит следующие элементы управления:
 *  - поле QDoubleSpinBox для задания минимального значения трения
 *  - поле QDoubleSpinBox для задания максимального значения трения
 *  - кнопку QButton "Отобразить графства" для применения фильтра
 *
 * @author  Марунин А.В.
 * @since   2.3
 */
class ddmMinMaxFrictionFilterWidget : public ddmFilterWidget
{
    Q_OBJECT

public:

    ddmMinMaxFrictionFilterWidget( ddmMinMaxFrictionFilter* filter, QWidget* parent = 0 );

    double minBound() const;
    void setMinBound( double bound );

    double maxBound() const;
    void setMaxBound( double bound );

    double minFriction() const;
    void setMinFiction( double value );

    double maxFriction() const;
    void setMaxFiction( double value );

    QDoubleSpinBox* spinMinValue() const;
    QDoubleSpinBox* spinMaxValue() const;

    virtual ~ddmMinMaxFrictionFilterWidget();

private:

    Ui::ddmMinMaxFrictionFilterWidget* ui;
};

#endif // DDM_MIN_MAX_FRICTION_FILTER_WIDGET_H
