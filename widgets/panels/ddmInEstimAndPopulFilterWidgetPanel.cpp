#include "widgets/panels/ddmInEstimAndPopulFilterWidgetPanel.h"
#include "ui_ddmInEstimAndPopulFilterWidgetPanel.h"


ddmInEstimAndPopulFilterWidgetPanel::ddmInEstimAndPopulFilterWidgetPanel( QWidget *parent ):
     QWidget( parent )
{
    this->ui = new Ui::ddmInEstimAndPopulFilterWidgetPanel;
    this->ui->setupUi( this );
    this->ui->gridLayout->setContentsMargins( 0,0,0,0 );
}


QDoubleSpinBox *ddmInEstimAndPopulFilterWidgetPanel::spinMinValue()
{
    return ui->spinMinValue;
}


QDoubleSpinBox *ddmInEstimAndPopulFilterWidgetPanel::spinMaxValue()
{
    return ui->spinMaxValue;
}


QPushButton *ddmInEstimAndPopulFilterWidgetPanel::applyButton()
{
    return ui->buttonApply;
}


ddmInEstimAndPopulFilterWidgetPanel::~ddmInEstimAndPopulFilterWidgetPanel()
{
    delete ui;
}
