#include "filters/ddmFrictionPopulationFilter.h"
#include "ui_ddmFrictionPopulationFilterWidget.h"
#include "widgets/ddmFrictionPopulationFilterWidget.h"


ddmFrictionPopulationFilterWidget::ddmFrictionPopulationFilterWidget( ddmFrictionPopulationFilter *filter, QWidget *parent ):
    ddmFilterWidget( filter, parent )
{
    this->ui = new Ui::ddmFrictionPopulationFilterWidget;
    this->ui->setupUi( this );

    QObject::connect( this->spinMinFrValue(), SIGNAL( valueChanged(double) ), this, SIGNAL( changed() ) );
    QObject::connect( this->spinMaxFrValue(), SIGNAL( valueChanged(double) ), this, SIGNAL( changed() ) );

    QObject::connect( this->spinMinPopValue(), SIGNAL( valueChanged(double) ), this, SIGNAL( changed() ) );
    QObject::connect( this->spinMaxPopValue(), SIGNAL( valueChanged(double) ), this, SIGNAL( changed() ) );


    QObject::connect( this->ui->buttonApply, SIGNAL( clicked() ), this, SLOT( slotApplyFilter() ) );
}


ddmFrictionPopulationFilterWidget::~ddmFrictionPopulationFilterWidget()
{
    delete ui;
}


double ddmFrictionPopulationFilterWidget::minFrictionBound() const
{
    return this->spinMinFrValue()->value();
}


void ddmFrictionPopulationFilterWidget::setMinFrictionBound( double bound )
{
    this->spinMinFrValue()->setValue( bound );
}


double ddmFrictionPopulationFilterWidget::maxFrictionBound() const
{
    return this->spinMaxFrValue()->value();
}


void ddmFrictionPopulationFilterWidget::setMaxFrictionBound( double bound )
{
    this->spinMaxFrValue()->setValue( bound );
}


double ddmFrictionPopulationFilterWidget::minFriction() const
{
    return this->spinMinFrValue()->minimum();
}


void ddmFrictionPopulationFilterWidget::setMinFiction( double value )
{
    this->spinMinFrValue()->setMinimum( value );
    this->spinMaxFrValue()->setMinimum( value );
}


double ddmFrictionPopulationFilterWidget::maxFriction() const
{
    return this->spinMaxFrValue()->maximum();
}


void ddmFrictionPopulationFilterWidget::setMaxFiction( double value )
{
    this->spinMinFrValue()->setMaximum( value );
    this->spinMaxFrValue()->setMaximum( value );
}


double ddmFrictionPopulationFilterWidget::minPopulationBound() const
{
    return this->spinMinPopValue()->value();
}


void ddmFrictionPopulationFilterWidget::setMinPopulationBound( double bound )
{
    this->spinMinPopValue()->setValue( bound );
}


double ddmFrictionPopulationFilterWidget::maxPopulationBound() const
{
    return this->spinMaxPopValue()->value();
}


void ddmFrictionPopulationFilterWidget::setMaxPopulationBound( double bound )
{
    return this->spinMaxPopValue()->setValue( bound );
}


void ddmFrictionPopulationFilterWidget::setMinPopulation( double value )
{
    this->spinMinPopValue()->setMinimum( value );
    this->spinMaxPopValue()->setMinimum( value );
}


double ddmFrictionPopulationFilterWidget::minPopulation() const
{
    return this->spinMinPopValue()->minimum();
}


double ddmFrictionPopulationFilterWidget::maxPopulation() const
{
    return this->spinMaxPopValue()->maximum();
}


void ddmFrictionPopulationFilterWidget::setMaxPopulation( double value )
{
    this->spinMinPopValue()->setMaximum( value );
    this->spinMaxPopValue()->setMaximum( value );
}


QDoubleSpinBox* ddmFrictionPopulationFilterWidget::spinMinFrValue() const
{
    return ui->m_spFrMinValue;
}


QDoubleSpinBox* ddmFrictionPopulationFilterWidget::spinMaxFrValue() const
{
    return ui->m_spFrMaxValue;
}


QDoubleSpinBox* ddmFrictionPopulationFilterWidget::spinMinPopValue() const
{
    return ui->m_spPopMinValue;
}


QDoubleSpinBox* ddmFrictionPopulationFilterWidget::spinMaxPopValue() const
{
    return ui->m_spPopMaxValue;
}
