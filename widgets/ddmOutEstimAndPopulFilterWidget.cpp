#include <QDoubleSpinBox>
#include <QPushButton>

#include "filters/ddmOutEstimAndPopulFilter.h"
#include "widgets/panels/ddmOutEstimAndPopulFilterWidgetPanel.h"
#include "widgets/ddmOutEstimAndPopulFilterWidget.h"



ddmOutEstimAndPopulFilterWidget::ddmOutEstimAndPopulFilterWidget( ddmOutEstimAndPopulFilter *filter, QWidget *parent ):
    ddmBaseCountyFilterWidget( filter, parent )
{
    this->m_panel = new ddmOutEstimAndPopulFilterWidgetPanel( this );
    this->insertWidget( this->panel() );

    QObject::connect( this->panel()->applyButton(), SIGNAL( clicked() ), this, SLOT( slotApplyFilter() ) );

    spinMinPopValue()->setMinimum( 0.0001 );
    spinMaxPopValue()->setMinimum( 0.0001 );

    spinMinPopValue()->setMaximum( 100.0 );
    spinMaxPopValue()->setMaximum( 100.0 );
}


ddmOutEstimAndPopulFilterWidgetPanel *ddmOutEstimAndPopulFilterWidget::panel() const
{
    return const_cast<ddmOutEstimAndPopulFilterWidgetPanel*>( this->m_panel );
}


void ddmOutEstimAndPopulFilterWidget::setMinBound( double bound )
{
  return this->spinMinPopValue()->setValue( bound );
}


void ddmOutEstimAndPopulFilterWidget::setMaxBound( double bound )
{
    return this->spinMaxPopValue()->setValue( bound );
}


QDoubleSpinBox* ddmOutEstimAndPopulFilterWidget::spinMinPopValue() const
{
    return this->m_panel->spinMinValue();
}


QDoubleSpinBox* ddmOutEstimAndPopulFilterWidget::spinMaxPopValue() const
{
    return this->m_panel->spinMaxValue();
}


double ddmOutEstimAndPopulFilterWidget::maxBound() const
{
   return this->spinMaxPopValue()->value();
}

double ddmOutEstimAndPopulFilterWidget::minBound() const
{
   return this->spinMinPopValue()->value();
}


ddmOutEstimAndPopulFilterWidget::~ddmOutEstimAndPopulFilterWidget()
{
    delete m_panel;
}
