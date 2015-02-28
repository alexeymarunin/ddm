#include <QLineEdit>
#include <QIntValidator>

#include "ddmMigrationCountyFilterWidget.h"
#include "ddmMigrationCountyFilter.h"
#include "ddmModel.h"
#include "ui_ddmMigrationCountyFilterWidget.h"

#include <limits.h>


ddmMigrationCountyFilterWidget::ddmMigrationCountyFilterWidget( ddmMigrationCountyFilter* filter, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ddmMigrationCountyFilterWidget)
{
    ui->setupUi(this);
    m_filter = filter;
    ui->verticalLayout_3->setContentsMargins( 0,0,0,0 );
    this->setContentsMargins( 0,0,0,0 );

    m_leState = new QLineEdit( ui->m_cmbState );
    m_leCounty = new QLineEdit( ui->m_cmbCounty );
    // инициализация начальных значений
    ui->m_cmbState->setLineEdit(  m_leState );
    ui->m_cmbCounty->setLineEdit( m_leCounty );

    QStringList states, counties, migrationMode;
    migrationMode.append( "Приехавшие" );
    migrationMode.append( "Уехавшие" );
    states = m_filter->model()->stateNames();
    counties = m_filter->model()->countyNames();
    ui->m_cmbState->addItems( states );
    ui->m_cmbCounty->addItems( counties );
    ui->m_cbViewMode->addItems( migrationMode );
    ui->groupBox->setEnabled( false );

    int minValue = 0, maxValue = std::numeric_limits<int>::max();;
    QIntValidator* leValidator = new QIntValidator( minValue, maxValue, this );
    ui->m_leFrom->setValidator( leValidator );
    ui->m_leTo->setValidator( leValidator );
    ui->m_leFrom->setText( "0" );
    ui->m_leTo->setText( "1000" );
    installEvents();
}


ddmMigrationCountyFilterWidget::~ddmMigrationCountyFilterWidget()
{
    delete ui;
}


void ddmMigrationCountyFilterWidget::installEvents()
{
    connect( ui->m_cbSetMigrationRange, SIGNAL( stateChanged( int ) ), SLOT( slotEnableMisgrationRange( int ) ) );
}


void ddmMigrationCountyFilterWidget::slotEnableMisgrationRange( int state )
{
    ui->groupBox->setEnabled( state );
}
