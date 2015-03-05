#include "filters/ddmMinMaxFrictionFilter.h"
#include "ui_ddmMinMaxFrictionFilterWidget.h"
#include "widgets/ddmMinMaxFrictionFilterWidget.h"

ddmMinMaxFrictionFilterWidget::ddmMinMaxFrictionFilterWidget( ddmMinMaxFrictionFilter* filter, QWidget* parent ) : ddmFilterWidget( filter, parent )
{
    this->ui = new Ui::ddmMinMaxFrictionFilterWidget;
    this->ui->setupUi( this );

    QObject::connect( this->spinMinValue(), SIGNAL( valueChanged(double) ), this, SIGNAL( changed() ) );
    QObject::connect( this->spinMaxValue(), SIGNAL( valueChanged(double) ), this, SIGNAL( changed() ) );

    QObject::connect( this->ui->buttonApply, SIGNAL( clicked() ), this, SLOT( slotApplyFilter() ) );
}

double ddmMinMaxFrictionFilterWidget::minBound() const
{
    return this->spinMinValue()->value();
}

void ddmMinMaxFrictionFilterWidget::setMinBound( double bound )
{
    this->spinMinValue()->setValue( bound );
}

double ddmMinMaxFrictionFilterWidget::maxBound() const
{
    return this->spinMaxValue()->value();
}

void ddmMinMaxFrictionFilterWidget::setMaxBound( double bound )
{
    this->spinMaxValue()->setValue( bound );
}

double ddmMinMaxFrictionFilterWidget::minFriction() const
{
    return this->spinMinValue()->minimum();
}

void ddmMinMaxFrictionFilterWidget::setMinFiction( double value )
{
    this->spinMinValue()->setMinimum( value );
    this->spinMaxValue()->setMinimum( value );
}

double ddmMinMaxFrictionFilterWidget::maxFriction() const
{
    return this->spinMaxValue()->maximum();
}

void ddmMinMaxFrictionFilterWidget::setMaxFiction( double value )
{
    this->spinMinValue()->setMaximum( value );
    this->spinMaxValue()->setMaximum( value );
}

QDoubleSpinBox* ddmMinMaxFrictionFilterWidget::spinMinValue() const
{
    return this->ui->spinMinValue;
}

QDoubleSpinBox* ddmMinMaxFrictionFilterWidget::spinMaxValue() const
{
    return this->ui->spinMaxValue;
}

ddmMinMaxFrictionFilterWidget::~ddmMinMaxFrictionFilterWidget()
{
    delete this->ui;
}
