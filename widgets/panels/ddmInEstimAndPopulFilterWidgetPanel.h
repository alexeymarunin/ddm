#ifndef DDM_IN_ESTIM_AND_POPUL_FILTER_WIDGET_PANEL_H
#define DDM_IN_ESTIM_AND_POPUL_FILTER_WIDGET_PANEL_H

#include <QWidget>


class QDoubleSpinBox;
class QPushButton;

namespace Ui {
    class ddmInEstimAndPopulFilterWidgetPanel;
}


class ddmInEstimAndPopulFilterWidgetPanel : public QWidget
{
    Q_OBJECT

public:

    ddmInEstimAndPopulFilterWidgetPanel(  QWidget* parent = NULL );

    virtual ~ddmInEstimAndPopulFilterWidgetPanel();

    QDoubleSpinBox* spinMinValue();
    QDoubleSpinBox* spinMaxValue();
    QPushButton* applyButton();

private:
    Ui::ddmInEstimAndPopulFilterWidgetPanel* ui;
};


#endif // DDM_IN_ESTIM_AND_POPUL_FILTER_WIDGET_PANEL_H

