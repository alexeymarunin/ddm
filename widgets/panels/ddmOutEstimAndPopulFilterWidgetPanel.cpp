#include "widgets/panels/ddmOutEstimAndPopulFilterWidgetPanel.h"
#include "ui_ddmOutEstimAndPopulFilterWidgetPanel.h"


ddmOutEstimAndPopulFilterWidgetPanel::ddmOutEstimAndPopulFilterWidgetPanel( QWidget *parent ):
     QWidget( parent )
{
    this->ui = new Ui::ddmOutEstimAndPopulFilterWidgetPanel;
    this->ui->setupUi( this );
    this->ui->gridLayout->setContentsMargins( 0,0,0,0 );
}


QDoubleSpinBox *ddmOutEstimAndPopulFilterWidgetPanel::spinMinValue()
{
    return ui->spinMinValue;
}


QDoubleSpinBox *ddmOutEstimAndPopulFilterWidgetPanel::spinMaxValue()
{
    return ui->spinMaxValue;
}


QPushButton *ddmOutEstimAndPopulFilterWidgetPanel::applyButton()
{
    return ui->buttonApply;
}


ddmOutEstimAndPopulFilterWidgetPanel::~ddmOutEstimAndPopulFilterWidgetPanel()
{
    delete ui;
}
