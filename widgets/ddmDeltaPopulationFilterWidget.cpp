#include "filters/ddmDeltaPopulationFilter.h"
#include "widgets/ddmDeltaPopulationFilterWidget.h"
#include "ui_ddmDeltaPopulationFilterWidget.h"


ddmDeltaPopulationFilterWidget::ddmDeltaPopulationFilterWidget( ddmDeltaPopulationFilter* filter, QWidget* parent ):
    ddmFilterWidget( filter, parent )
{
    this->ui = new Ui::ddmDeltaPopulationFilterWidget;
    this->ui->setupUi( this );

    ui->horizontalLayout->setContentsMargins( 0,0,0,0 );
    this->setContentsMargins( 0,0,0,0 );

    QObject::connect( this->spinMinValue(), SIGNAL( valueChanged(double) ), this, SIGNAL( changed() ) );
    QObject::connect( this->spinMaxValue(), SIGNAL( valueChanged(double) ), this, SIGNAL( changed() ) );

    QObject::connect( this->ui->buttonApply, SIGNAL( clicked() ), this, SLOT( slotApplyFilter() ) );
}


double ddmDeltaPopulationFilterWidget::minBound() const
{
    return spinMinValue()->value();
}


void ddmDeltaPopulationFilterWidget::setMinBound( double bound )
{
    return spinMinValue()->setValue( bound );
}


double ddmDeltaPopulationFilterWidget::maxBound() const
{
    return spinMaxValue()->value();
}


void ddmDeltaPopulationFilterWidget::setMaxBound( double bound )
{
    spinMaxValue()->setValue( bound );
}


void ddmDeltaPopulationFilterWidget::setMaxPopulation( double value )
{
    spinMinValue()->setMaximum( value );
    spinMaxValue()->setMaximum( value );
}


void ddmDeltaPopulationFilterWidget::setMinPopulation( double value )
{
    spinMinValue()->setMinimum( value );
    spinMaxValue()->setMinimum( value );
}


double ddmDeltaPopulationFilterWidget::minPopulation()
{
    return spinMinValue()->minimum();
}


double ddmDeltaPopulationFilterWidget::maxPopulation()
{
    return spinMaxValue()->maximum();
}


QDoubleSpinBox *ddmDeltaPopulationFilterWidget::spinMinValue() const
{
    return ui->spinMinValue;
}


QDoubleSpinBox *ddmDeltaPopulationFilterWidget::spinMaxValue() const
{
    return ui->spinMaxValue;
}


ddmDeltaPopulationFilterWidget::~ddmDeltaPopulationFilterWidget()
{
    delete ui;

}
