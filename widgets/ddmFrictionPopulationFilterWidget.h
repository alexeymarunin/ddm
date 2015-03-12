#ifndef DDM_FRICTION_POPULATION_FILTER_WIDGET_H
#define DDM_FRICTION_POPULATION_FILTER_WIDGET_H

#include "widgets/ddmFilterWidget.h"


namespace Ui {
    class ddmFrictionPopulationFilterWidget;
}

class QDoubleSpinBox;
class ddmFrictionPopulationFilter;


class ddmFrictionPopulationFilterWidget: public ddmFilterWidget
{
    Q_OBJECT

 public:
    ddmFrictionPopulationFilterWidget( ddmFrictionPopulationFilter* filter, QWidget* parent = 0 );

    ~ddmFrictionPopulationFilterWidget();

    double minFrictionBound() const;
    void setMinFrictionBound( double bound );

    double maxFrictionBound() const;
    void setMaxFrictionBound( double bound );

    double minFriction() const;
    void setMinFiction( double value );

    double maxFriction() const;
    void setMaxFiction( double value );



    double minPopulationBound() const;
    void setMinPopulationBound( double bound );

    double maxPopulationBound() const;
    void setMaxPopulationBound( double bound );

    double minPopulation() const;
    void setMinPopulation( double value );

    double maxPopulation() const;
    void setMaxPopulation( double value );



    QDoubleSpinBox* spinMinFrValue() const;
    QDoubleSpinBox* spinMaxFrValue() const;

    QDoubleSpinBox* spinMinPopValue() const;
    QDoubleSpinBox* spinMaxPopValue() const;


private:

    Ui::ddmFrictionPopulationFilterWidget* ui;

};

#endif // DDM_FRICTION_POPULATION_FILTER_WIDGET_H

