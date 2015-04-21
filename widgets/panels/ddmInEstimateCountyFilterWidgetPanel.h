#ifndef DDM_IN_ESTIMATE_COUNTY_FILTER_WIDGET_PANEL_H
#define DDM_IN_ESTIMATE_COUNTY_FILTER_WIDGET_PANEL_H

#include <QWidget>

class QDoubleSpinBox;
class QPushButton;

namespace Ui {
    class ddmInEstimateCountyFilterWidgetPanel;
}


class ddmInEstimateCountyFilterWidgetPanel : public QWidget
{
    Q_OBJECT

public:

    ddmInEstimateCountyFilterWidgetPanel( QWidget* parent = NULL );

    virtual ~ddmInEstimateCountyFilterWidgetPanel();

    QDoubleSpinBox* spinMinPopValue();
    QDoubleSpinBox* spinMaxPopValue();
    QPushButton* applyButton();

private:

    Ui::ddmInEstimateCountyFilterWidgetPanel* ui;
};


#endif // DDM_IN_ESTIMATE_COUNTY_FILTER_WIDGET_PANEL_H

