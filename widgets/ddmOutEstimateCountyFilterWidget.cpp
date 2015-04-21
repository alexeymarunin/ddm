#include <QDoubleSpinBox>
#include <QPushButton>

#include "widgets/panels/ddmOutEstimateCountyFilterWidgetPanel.h"
#include "widgets/ddmOutEstimateCountyFilterWidget.h"
#include "filters/ddmOutEstimateCountyFilter.h"



ddmOutEstimateCountyFilterWidget::ddmOutEstimateCountyFilterWidget( ddmOutEstimateCountyFilter* filter, QWidget* parent ):
    ddmBaseCountyFilterWidget( filter, parent )
{
    this->m_panel = new ddmOutEstimateCountyFilterWidgetPanel( this );
    this->insertWidget( this->panel() );

    QObject::connect( this->panel()->applyButton(), SIGNAL( clicked() ), this, SLOT( slotApplyFilter() ) );
}


ddmOutEstimateCountyFilterWidgetPanel* ddmOutEstimateCountyFilterWidget::panel() const
{
    return const_cast<ddmOutEstimateCountyFilterWidgetPanel*>( this->m_panel );
}


double ddmOutEstimateCountyFilterWidget::minPopulationBound() const
{
    return this->spinMinPopValue()->value();
}


void ddmOutEstimateCountyFilterWidget::setMinPopulationBound( double bound )
{
    return this->spinMinPopValue()->setValue( bound );
}


double ddmOutEstimateCountyFilterWidget::maxPopulationBound() const
{
    return this->spinMaxPopValue()->value();
}


void ddmOutEstimateCountyFilterWidget::setMaxPopulationBound( double bound )
{
    return this->spinMaxPopValue()->setValue( bound );
}


double ddmOutEstimateCountyFilterWidget::minPopulation() const
{
    return this->spinMinPopValue()->minimum();
}


void ddmOutEstimateCountyFilterWidget::setMinPopulation( double value )
{
    return this->spinMinPopValue()->setMinimum( value );
    return this->spinMaxPopValue()->setMinimum( value );
}


double ddmOutEstimateCountyFilterWidget::maxPopulation() const
{
    return this->spinMaxPopValue()->maximum();
}


void ddmOutEstimateCountyFilterWidget::setMaxPopulation( double value )
{
    return this->spinMinPopValue()->setMaximum( value );
    return this->spinMaxPopValue()->setMaximum( value );
}


QDoubleSpinBox *ddmOutEstimateCountyFilterWidget::spinMinPopValue() const
{
    return panel()->spinMinPopValue();
}


QDoubleSpinBox *ddmOutEstimateCountyFilterWidget::spinMaxPopValue() const
{
    return panel()->spinMaxPopValue();
}


ddmOutEstimateCountyFilterWidget::~ddmOutEstimateCountyFilterWidget()
{

}
