#include "filters/ddmFrictionDeltaFilter.h"
#include "ui_ddmFrictionDeltaFilterWidget.h"
#include "widgets/ddmFrictionDeltaFilterWidget.h"


ddmFrictionDeltaFilterWidget::ddmFrictionDeltaFilterWidget( ddmFrictionDeltaFilter *filter, QWidget *parent) :
     ddmFilterWidget( filter, parent )
{
    this->ui = new Ui::ddmFrictionDeltaFilterWidget;
    this->ui->setupUi( this );
    this->ui->verticalLayout_2->setContentsMargins( 0,0,0,0 );
    this->setContentsMargins( 0,0,0,0 );
    this->ui->m_cbDeltaMode->addItem( "положительная дельта" );
    this->ui->m_cbDeltaMode->addItem( "отрицательная дельта" );
    this->ui->m_cbDeltaMode->setCurrentIndex( 0 );
    this->ui->m_cbDeltaMode->setCurrentText( "положительная дельта" );

    QObject::connect( this->spinMinValue(), SIGNAL( valueChanged(double) ), this, SIGNAL( changed() ) );
    QObject::connect( this->spinMaxValue(), SIGNAL( valueChanged(double) ), this, SIGNAL( changed() ) );
    QObject::connect( this->ui->m_cbDeltaMode, SIGNAL( currentIndexChanged(int) ), this, SLOT( slotApplyFilter() ) );
    QObject::connect( this->ui->buttonApply, SIGNAL( clicked() ), this, SLOT( slotApplyFilter() ) );
}



double ddmFrictionDeltaFilterWidget::minBound() const
{
   return this->spinMinValue()->value();
}


void ddmFrictionDeltaFilterWidget::setMinBound( double bound )
{
    this->spinMaxValue()->setValue( bound );

}


double ddmFrictionDeltaFilterWidget::maxBound() const
{
    return this->spinMaxValue()->value();
}


void ddmFrictionDeltaFilterWidget::setMaxBound( double bound )
{
    this->spinMaxValue()->setValue( bound );
}


double ddmFrictionDeltaFilterWidget::minFriction() const
{
    return this->spinMinValue()->minimum();
}


void ddmFrictionDeltaFilterWidget::setMinFiction( double value )
{
    this->spinMinValue()->setMinimum( value );
    this->spinMaxValue()->setMinimum( value );
}


double ddmFrictionDeltaFilterWidget::maxFriction() const
{
    return this->spinMaxValue()->maximum();
}


void ddmFrictionDeltaFilterWidget::setMaxFiction( double value )
{
    this->spinMinValue()->setMaximum( value );
    this->spinMaxValue()->setMaximum( value );
}


void ddmFrictionDeltaFilterWidget::setDeltaMode( int mode )
{
    this->comboDeltaMode()->setCurrentIndex( mode );
}


int ddmFrictionDeltaFilterWidget::deltaMode() const
{
    return this->comboDeltaMode()->currentIndex();
}


QComboBox *ddmFrictionDeltaFilterWidget::comboDeltaMode() const
{
    return ui->m_cbDeltaMode;
}


QDoubleSpinBox *ddmFrictionDeltaFilterWidget::spinMinValue() const
{
    return ui->spinMinValue;
}


QDoubleSpinBox *ddmFrictionDeltaFilterWidget::spinMaxValue() const
{
    return ui->spinMaxValue;
}

ddmFrictionDeltaFilterWidget::~ddmFrictionDeltaFilterWidget()
{
    delete ui;
}

