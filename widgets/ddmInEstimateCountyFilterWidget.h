#ifndef DDM_IN_ESTIMATE_COUNTY_FILTER_WIDGET_H
#define DDM_IN_ESTIMATE_COUNTY_FILTER_WIDGET_H

#include "widgets/ddmBaseCountyFilterWidget.h"


class QDoubleSpinBox;

class ddmInEstimateCountyFilter;
class ddmInEstimateCountyFilterWidgetPanel;


class ddmInEstimateCountyFilterWidget : public ddmBaseCountyFilterWidget
{
    Q_OBJECT

public:

    ddmInEstimateCountyFilterWidget( ddmInEstimateCountyFilter* filter, QWidget* parent = NULL );

    ddmInEstimateCountyFilterWidgetPanel* panel() const;

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


    virtual ~ddmInEstimateCountyFilterWidget();

private:
    ddmInEstimateCountyFilterWidgetPanel* m_panel;
};

#endif // DDM_IN_ESTIMATE_COUNTY_FILTER_H

