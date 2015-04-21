#include "widgets/panels/ddmInEstimateCountyFilterWidgetPanel.h"
#include "ui_ddmInEstimateCountyFilterWidgetPanel.h"


ddmInEstimateCountyFilterWidgetPanel::ddmInEstimateCountyFilterWidgetPanel( QWidget* parent ) : QWidget( parent )
{
    this->ui = new Ui::ddmInEstimateCountyFilterWidgetPanel;
    this->ui->setupUi( this );
    this->ui->verticalLayout->setContentsMargins( 0,0,0,0 );
}


QDoubleSpinBox *ddmInEstimateCountyFilterWidgetPanel::spinMinPopValue()
{
    return ui->spinMinValue;
}


QDoubleSpinBox *ddmInEstimateCountyFilterWidgetPanel::spinMaxPopValue()
{
    return ui->spinMaxValue;
}


QPushButton *ddmInEstimateCountyFilterWidgetPanel::applyButton()
{
    return ui->buttonApply;
}


ddmInEstimateCountyFilterWidgetPanel::~ddmInEstimateCountyFilterWidgetPanel()
{
    delete ui;
}

