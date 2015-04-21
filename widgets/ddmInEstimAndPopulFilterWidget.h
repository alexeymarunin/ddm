#ifndef DDM_IN_ESTIM_AND_POPUL_FILTER_WIDGET_H
#define DDM_IN_ESTIM_AND_POPUL_FILTER_WIDGET_H

#include "widgets/ddmBaseCountyFilterWidget.h"

class QDoubleSpinBox;

class ddmInEstimAndPopulFilter;
class ddmInEstimAndPopulFilterWidgetPanel;


class ddmInEstimAndPopulFilterWidget : public ddmBaseCountyFilterWidget
{
    Q_OBJECT

public:

    ddmInEstimAndPopulFilterWidget( ddmInEstimAndPopulFilter* filter, QWidget* parent = NULL );

    ddmInEstimAndPopulFilterWidgetPanel* panel() const;

    double minBound() const;
    void setMinBound( double bound );

    double maxBound() const;
    void setMaxBound( double bound );

    double minPopulation() const;
    double maxPopulation() const;

    QDoubleSpinBox* spinMinPopValue() const;
    QDoubleSpinBox* spinMaxPopValue() const;


    virtual ~ddmInEstimAndPopulFilterWidget();

private:
    ddmInEstimAndPopulFilterWidgetPanel* m_panel;
};


#endif // DDM_IN_ESTIM_AND_POPUL_FILTER_WIDGET_H

