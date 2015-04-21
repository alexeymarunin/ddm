#ifndef DDM_OUT_ESTIM_AND_POPUL_FILTER_WIDGET_PANEL_H
#define DDM_OUT_ESTIM_AND_POPUL_FILTER_WIDGET_PANEL_H

#include <QWidget>


class QDoubleSpinBox;
class QPushButton;

namespace Ui {
    class ddmOutEstimAndPopulFilterWidgetPanel;
}


class ddmOutEstimAndPopulFilterWidgetPanel : public QWidget
{
    Q_OBJECT

public:

    ddmOutEstimAndPopulFilterWidgetPanel(  QWidget* parent = NULL );

    virtual ~ddmOutEstimAndPopulFilterWidgetPanel();

    QDoubleSpinBox* spinMinValue();
    QDoubleSpinBox* spinMaxValue();
    QPushButton* applyButton();

private:
    Ui::ddmOutEstimAndPopulFilterWidgetPanel* ui;
};


#endif // DDM_OUT_ESTIM_AND_POPUL_FILTER_WIDGET_PANEL_H

