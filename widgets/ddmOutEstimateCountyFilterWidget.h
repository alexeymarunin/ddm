#ifndef DDM_IN_ESTIMATE_COUNTY_FILTER_WIDGET_H
#define DDM_IN_ESTIMATE_COUNTY_FILTER_WIDGET_H

#include "widgets/ddmBaseCountyFilterWidget.h"


class QDoubleSpinBox;

class ddmOutEstimateCountyFilter;
class ddmOutEstimateCountyFilterWidgetPanel;


class ddmOutEstimateCountyFilterWidget : public ddmBaseCountyFilterWidget
{
    Q_OBJECT

public:

    ddmOutEstimateCountyFilterWidget( ddmOutEstimateCountyFilter* filter, QWidget* parent = NULL );

    ddmOutEstimateCountyFilterWidgetPanel* panel() const;

    double minPopulationBound() const;
    void setMinPopulationBound( double bound );

    double maxPopulationBound() const;
    void setMaxPopulationBound( double bound );

    double minPopulation() const;
    void setMinPopulation( double value );

    double maxPopulation() const;
    void setMaxPopulation( double value );


    QDoubleSpinBox* spinMinPopValue() const;
    QDoubleSpinBox* spinMaxPopValue() const;


    virtual ~ddmOutEstimateCountyFilterWidget();

private:
    ddmOutEstimateCountyFilterWidgetPanel* m_panel;
};

#endif // DDM_IN_ESTIMATE_COUNTY_FILTER_H

