#ifndef DDM_IN_ESTIM_AND_POPUL_FILTER_WIDGET_H
#define DDM_IN_ESTIM_AND_POPUL_FILTER_WIDGET_H

#include "widgets/ddmBaseCountyFilterWidget.h"

class QDoubleSpinBox;

class ddmOutEstimAndPopulFilter;
class ddmOutEstimAndPopulFilterWidgetPanel;


class ddmOutEstimAndPopulFilterWidget : public ddmBaseCountyFilterWidget
{
    Q_OBJECT

public:

    ddmOutEstimAndPopulFilterWidget( ddmOutEstimAndPopulFilter* filter, QWidget* parent = NULL );

    ddmOutEstimAndPopulFilterWidgetPanel* panel() const;

    double minBound() const;
    void setMinBound( double bound );

    double maxBound() const;
    void setMaxBound( double bound );

    double minPopulation() const;
    double maxPopulation() const;

    QDoubleSpinBox* spinMinPopValue() const;
    QDoubleSpinBox* spinMaxPopValue() const;


    virtual ~ddmOutEstimAndPopulFilterWidget();

private:
    ddmOutEstimAndPopulFilterWidgetPanel* m_panel;
};


#endif // DDM_OUT_ESTIM_AND_POPUL_FILTER_WIDGET_H

