#include "ddmFrictionCountyFilterWidget.h"
#include "ui_ddmfrictioncountyfilterwidget.h"

ddmFrictionCountyFilterWidget::ddmFrictionCountyFilterWidget( ddmFrictionCountyFilter* filter, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ddmFrictionCountyFilterWidget)
{
    ui->setupUi(this);
    ui->gridLayout->setContentsMargins( 0,0,0,0 );
    ui->gridLayout_2->setContentsMargins( 0,0,0,0 );
    ui->gridLayout_3->setContentsMargins( 0,0,0,0 );
    this->setContentsMargins( 0,0,0,0 );

    ui->m_rbIntervalMode->setChecked( true );
    ui->m_lbValue->setEnabled( false );
    ui->m_leValue->setEnabled( false );


    m_filter = filter;
    installEvents();
}

ddmFrictionCountyFilterWidget::~ddmFrictionCountyFilterWidget()
{
    delete ui;
}


void ddmFrictionCountyFilterWidget::slotValueMode( bool state )
{
   ui->m_lbValue->setEnabled( state );
   ui->m_leValue->setEnabled( state );
}


void ddmFrictionCountyFilterWidget::slotIntervalMode( bool state )
{
    ui->m_lbIntervalMode->setEnabled( state );
    ui->m_lbFrom->setEnabled( state );
    ui->m_lbTo->setEnabled( state );
    ui->m_leFrom->setEnabled( state );
    ui->m_leTo->setEnabled( state );
}


void ddmFrictionCountyFilterWidget::installEvents()
{
    connect( ui->m_rbValueMode, SIGNAL( toggled( bool ) ), this, SLOT( slotValueMode( bool ) ) );
    connect( ui->m_rbIntervalMode, SIGNAL( toggled( bool ) ), this, SLOT( slotIntervalMode( bool ) ) );

}
