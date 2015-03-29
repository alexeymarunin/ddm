#ifndef DDM_OUT_ESTIMATE_COUNTY_FILTER_WIDGET_PANEL_H
#define DDM_OUT_ESTIMATE_COUNTY_FILTER_WIDGET_PANEL_H

#include <QWidget>

class QDoubleSpinBox;
class QPushButton;

namespace Ui {
    class ddmOutEstimateCountyFilterWidgetPanel;
}


class ddmOutEstimateCountyFilterWidgetPanel : public QWidget
{
    Q_OBJECT

public:

    ddmOutEstimateCountyFilterWidgetPanel(  QWidget* parent = NULL );

    virtual ~ddmOutEstimateCountyFilterWidgetPanel();

    QDoubleSpinBox* spinMinPopValue();
    QDoubleSpinBox* spinMaxPopValue();
    QPushButton* applyButton();

private:

    Ui::ddmOutEstimateCountyFilterWidgetPanel* ui;
};


#endif // DDM_OUT_ESTIMATE_COUNTY_FILTER_WIDGET_PANEL_H
