#include "widgets\ddmPosNegDeltaFilterWidget.h"
#include "filters\ddmPosNegDeltaFilter.h"
#include "ui_ddmPosNegDeltaFilterWidget.h"


ddmPosNegDeltaFilterWidget::ddmPosNegDeltaFilterWidget( ddmPosNegDeltaFilter* filter, QWidget* parent ) : ddmFilterWidget( filter, parent )
{
    this->ui = new Ui::ddmPosNegDeltaFilterWidget;
    this->ui->setupUi( this );
    this->ui->verticalLayout_2->setContentsMargins( 0,0,0,0 );
    this->setContentsMargins( 0,0,0,0 );
    this->ui->m_cbDeltaMode->addItem( "положительная дельта" );
    this->ui->m_cbDeltaMode->addItem( "отрицательная дельта" );
    QObject::connect( this->ui->m_cbDeltaMode, SIGNAL( currentIndexChanged(int) ), this, SLOT( slotApplyFilter() ) );
}


void ddmPosNegDeltaFilterWidget::setDeltaMode( int mode )
{
    this->comboDeltaMode()->setCurrentIndex( mode );
}


int ddmPosNegDeltaFilterWidget::deltaMode() const
{
    return this->comboDeltaMode()->currentIndex();
}


QComboBox *ddmPosNegDeltaFilterWidget::comboDeltaMode() const
{
    return ui->m_cbDeltaMode;
}

ddmPosNegDeltaFilterWidget::~ddmPosNegDeltaFilterWidget()
{
    delete ui;
}

