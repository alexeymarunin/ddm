#include <QDoubleSpinBox>
#include <QPushButton>

#include "filters/ddmInEstimAndPopulFilter.h"
#include "widgets/panels/ddmInEstimAndPopulFilterWidgetPanel.h"
#include "widgets/ddmInEstimAndPopulFilterWidget.h"



ddmInEstimAndPopulFilterWidget::ddmInEstimAndPopulFilterWidget( ddmInEstimAndPopulFilter *filter, QWidget *parent ):
    ddmBaseCountyFilterWidget( filter, parent )
{
    this->m_panel = new ddmInEstimAndPopulFilterWidgetPanel( this );
    this->insertWidget( this->panel() );

    QObject::connect( this->panel()->applyButton(), SIGNAL( clicked() ), this, SLOT( slotApplyFilter() ) );

    spinMinPopValue()->setMinimum( 0.0001 );
    spinMaxPopValue()->setMinimum( 0.0001 );

    spinMinPopValue()->setMaximum( 100.0 );
    spinMaxPopValue()->setMaximum( 100.0 );
}


ddmInEstimAndPopulFilterWidgetPanel *ddmInEstimAndPopulFilterWidget::panel() const
{
    return const_cast<ddmInEstimAndPopulFilterWidgetPanel*>( this->m_panel );
}


void ddmInEstimAndPopulFilterWidget::setMinBound( double bound )
{
  return this->spinMinPopValue()->setValue( bound );
}


void ddmInEstimAndPopulFilterWidget::setMaxBound( double bound )
{
    return this->spinMaxPopValue()->setValue( bound );
}


QDoubleSpinBox* ddmInEstimAndPopulFilterWidget::spinMinPopValue() const
{
    return this->m_panel->spinMinValue();
}


QDoubleSpinBox* ddmInEstimAndPopulFilterWidget::spinMaxPopValue() const
{
    return this->m_panel->spinMaxValue();
}


double ddmInEstimAndPopulFilterWidget::maxBound() const
{
   return this->spinMaxPopValue()->value();
}

double ddmInEstimAndPopulFilterWidget::minBound() const
{
   return this->spinMinPopValue()->value();
}


ddmInEstimAndPopulFilterWidget::~ddmInEstimAndPopulFilterWidget()
{
    delete m_panel;
}
