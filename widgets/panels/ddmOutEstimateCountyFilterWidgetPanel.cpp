#include "widgets/panels/ddmOutEstimateCountyFilterWidgetPanel.h"
#include "ui_ddmOutEstimateCountyFilterWidgetPanel.h"


ddmOutEstimateCountyFilterWidgetPanel::ddmOutEstimateCountyFilterWidgetPanel( QWidget *parent ) :
    QWidget( parent )
{
    this->ui = new Ui::ddmOutEstimateCountyFilterWidgetPanel;
    this->ui->setupUi( this );
    this->ui->gridLayout->setContentsMargins( 0,0,0,0 );
}


QDoubleSpinBox* ddmOutEstimateCountyFilterWidgetPanel::spinMinPopValue()
{
    return ui->spinMinValue;
}


QDoubleSpinBox* ddmOutEstimateCountyFilterWidgetPanel::spinMaxPopValue()
{
    return ui->spinMaxValue;
}


QPushButton* ddmOutEstimateCountyFilterWidgetPanel::applyButton()
{
    return ui->buttonApply;
}


ddmOutEstimateCountyFilterWidgetPanel::~ddmOutEstimateCountyFilterWidgetPanel()
{
    delete ui;
}

