#ifndef DDM_FRICTION_DELTA_MODEL_WIDGET_H
#define DDM_FRICTION_DELTA_MODEL_WIDGET_H

#include "ddmFilterWidget.h"


namespace Ui {
    class ddmFrictionDeltaFilterWidget;
}

class ddmFrictionDeltaFilter;
class QDoubleSpinBox;
class QComboBox;


class ddmFrictionDeltaFilterWidget: public ddmFilterWidget
{
    Q_OBJECT
public:

    ddmFrictionDeltaFilterWidget( ddmFrictionDeltaFilter* filter, QWidget* parent = 0 );
    ~ddmFrictionDeltaFilterWidget();

    double minBound() const;
    void setMinBound( double bound );

    double maxBound() const;
    void setMaxBound( double bound );

    double minFriction() const;
    void setMinFiction( double value );

    double maxFriction() const;
    void setMaxFiction( double value );

    void setDeltaMode( int mode );

    int deltaMode() const;

    QComboBox* comboDeltaMode() const;

    QDoubleSpinBox* spinMinValue() const;
    QDoubleSpinBox* spinMaxValue() const;

private:
    Ui::ddmFrictionDeltaFilterWidget* ui;
};

#endif // DDM_FRICTION_DELTA_MODEL_WIDGET_H

