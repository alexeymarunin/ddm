#include <QLineEdit>

#include "ddmEmptyFlterWidget.h"
#include "ui_ddmEmptyFilterWidget.h"
#include "ddmEmptyfilter.h"
#include "ddmModel.h"
#include "ddmMapView.h"


ddmEmptyFilterWidget::ddmEmptyFilterWidget( ddmEmptyFilter *filter, QWidget *parent ) :
    QWidget(parent),
    ui(new Ui::ddmEmptyFilterWidget)
{
    ui->setupUi(this);
    ui->gridLayout->setContentsMargins(0,0,0,0);
    this->setContentsMargins(0,0,0,0);
    m_filter = filter;

    m_leState = new QLineEdit( ui->m_cmbState );
    m_leCounty = new QLineEdit( ui->m_cmbCounty );
    // инициализация начальных значений
    ui->m_cmbState->setLineEdit(  m_leState );
    ui->m_cmbCounty->setLineEdit( m_leCounty );

    QStringList states, counties;
    states = m_filter->model()->stateNames();
    counties = m_filter->model()->countyNames();
    ui->m_cmbState->addItems( states );
    ui->m_cmbCounty->addItems( counties );

    this->slotSetCurrentState(  m_filter->model()->currentState()->geographicName() );
    this->slotSetCurrentCounty( m_filter->model()->currentCounty()->geographicName() );

    installEvents();
}


ddmEmptyFilterWidget::~ddmEmptyFilterWidget()
{
    delete ui;
    m_filter = NULL;
}


void ddmEmptyFilterWidget::slotSetCurrentState( const QString &stateName )
{
    m_filter->model()->setCurrentState( stateName );
    this->updateCountiesList();
}


void ddmEmptyFilterWidget::slotSetCurrentCounty( const QString &countyName )
{
    m_filter->model()->setCurrentCounty( countyName );
}


void ddmEmptyFilterWidget::changedState( ddmState *state )
{
     this->slotSetCurrentState( state->geographicName() );
     ui->m_cmbState->clearEditText();
     ui->m_cmbState->setEditText( m_filter->model()->currentState()->geographicName() );
}


void ddmEmptyFilterWidget::changedCounty( ddmCounty *county )
{
     if( county )
     {
         this->slotSetCurrentCounty( county->geographicName() );
         m_filter->mapView()->selectCounty( county->id() );
         ui->m_cmbCounty->setEditText( m_filter->model()->currentCounty()->geographicName() );
     }
}


void ddmEmptyFilterWidget::installEvents()
{
    connect( ui->m_cmbState,  SIGNAL( activated ( const QString& ) ), this,  SLOT( slotSetCurrentState( const QString& ) ));
    connect( ui->m_cmbCounty, SIGNAL( activated ( const QString& ) ), this, SLOT(  slotSetCurrentCounty( const QString& ) ) );
    connect( m_filter->model(), SIGNAL( changedState( ddmState* ) ), this, SLOT( changedState( ddmState* ) ), Qt::UniqueConnection );
    connect( m_filter->model(), SIGNAL( changedCounty( ddmCounty* ) ), this, SLOT( changedCounty( ddmCounty* ) ) );
}


void ddmEmptyFilterWidget::updateCountiesList()
{
    QStringList counties( m_filter->model()->countyNames() );
    ui->m_cmbCounty->clear();
    ui->m_cmbCounty->clearEditText();
    ui->m_cmbCounty->addItems( counties );
    ui->m_cmbCounty->setEditText( m_filter->model()->currentCounty()->geographicName() );
}
