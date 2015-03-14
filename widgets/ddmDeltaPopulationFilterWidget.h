#ifndef DDM_DELTA_POPULATION_FILTER_WIDGET_H
#define DDM_DELTA_POPULATION_FILTER_WIDGET_H

#include "widgets/ddmFilterWidget.h"


namespace Ui {
    class ddmDeltaPopulationFilterWidget;
}


class ddmDeltaPopulationFilter;
class QDoubleSpinBox;


class ddmDeltaPopulationFilterWidget: public ddmFilterWidget
{
    Q_OBJECT

public:

    ddmDeltaPopulationFilterWidget( ddmDeltaPopulationFilter* filter, QWidget* parent = 0 );

    double minBound() const;
    void setMinBound( double bound );

    double maxBound() const;
    void setMaxBound( double bound );

    void setMaxPopulation( double value );
    double maxPopulation();

    void setMinPopulation( double value );
    double minPopulation();

    QDoubleSpinBox* spinMinValue() const;
    QDoubleSpinBox* spinMaxValue() const;

    virtual ~ddmDeltaPopulationFilterWidget();

private:

    Ui::ddmDeltaPopulationFilterWidget* ui;
};

#endif // DDM_DELTA_POPULATION_FILTER_WIDGET_H

