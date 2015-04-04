#include <QDoubleSpinBox>
#include <QPushButton>

#include "widgets/panels/ddmInEstimateCountyFilterWidgetPanel.h"
#include "widgets/ddmInEstimateCountyFilterWidget.h"
#include "filters/ddmInEstimateCountyFilter.h"



ddmInEstimateCountyFilterWidget::ddmInEstimateCountyFilterWidget( ddmInEstimateCountyFilter *filter, QWidget *parent ):
    ddmBaseCountyFilterWidget( filter, parent )
{
    this->m_panel = new ddmInEstimateCountyFilterWidgetPanel( this );
    this->insertWidget( this->panel() );



    QObject::connect( this->panel()->applyButton(), SIGNAL( clicked() ), this, SLOT( slotApplyFilter() ) );
}


ddmInEstimateCountyFilterWidgetPanel* ddmInEstimateCountyFilterWidget::panel() const
{
    return const_cast<ddmInEstimateCountyFilterWidgetPanel*>( this->m_panel );
}


double ddmInEstimateCountyFilterWidget::minPopulationBound() const
{
    return this->spinMinPopValue()->value();
}


void ddmInEstimateCountyFilterWidget::setMinPopulationBound( double bound )
{
    return this->spinMinPopValue()->setValue( bound );
}


double ddmInEstimateCountyFilterWidget::maxPopulationBound() const
{
    return this->spinMaxPopValue()->value();
}


void ddmInEstimateCountyFilterWidget::setMaxPopulationBound( double bound )
{
    return this->spinMaxPopValue()->setValue( bound );
}


double ddmInEstimateCountyFilterWidget::minPopulation() const
{
    return this->spinMinPopValue()->minimum();
}


void ddmInEstimateCountyFilterWidget::setMinPopulation( double value )
{
    return this->spinMinPopValue()->setMinimum( value );
    return this->spinMaxPopValue()->setMinimum( value );
}


double ddmInEstimateCountyFilterWidget::maxPopulation() const
{
    return this->spinMaxPopValue()->maximum();
}


void ddmInEstimateCountyFilterWidget::setMaxPopulation( double value )
{
    return this->spinMinPopValue()->setMaximum( value );
    return this->spinMaxPopValue()->setMaximum( value );
}


QDoubleSpinBox *ddmInEstimateCountyFilterWidget::spinMinPopValue() const
{
    ddmInEstimateCountyFilterWidgetPanel* panel = this->panel();
    return panel->spinMinPopValue();
}


QDoubleSpinBox *ddmInEstimateCountyFilterWidget::spinMaxPopValue() const
{
    return  panel()->spinMaxPopValue();
}


ddmInEstimateCountyFilterWidget::~ddmInEstimateCountyFilterWidget()
{

}
