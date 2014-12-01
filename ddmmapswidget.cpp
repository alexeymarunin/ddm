#include "ddmmapswidget.h"
#include "ui_ddmmapswidget.h"
#include "ddmObjectModel.h"

#include <QLineEdit>

ddmMapsWidget::ddmMapsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ddmMapsWidget)
{
    ui->setupUi(this);

    m_leState = new QLineEdit( ui->m_cmbState );
    m_leCounty = new QLineEdit( ui->m_cmbCounty );

    m_model = new ddmObjectModel();

    ui->m_cmbState->setLineEdit( m_leState );
    ui->m_cmbCounty->setLineEdit( m_leCounty );

    connect( ui->m_cmbState,  SIGNAL( activated ( const QString& ) ), this,  SLOT( slotSetCurentState( const QString& ) ));
    connect( ui->m_cmbCounty, SIGNAL( activated ( const QString& ) ),  this, SLOT( slotSetCurentState( const QString& ) ) );

    connect( m_leState,  SIGNAL( editingFinished() ), this, SLOT(  slotSetCurentState() ));
    connect( m_leCounty, SIGNAL( editingFinished() ),  this, SLOT( slotSetCurentCounty() ) );
}

ddmMapsWidget::~ddmMapsWidget()
{
    delete m_model;
    delete ui;
}


void ddmMapsWidget::slotSetCurentState( const QString& text )
{
    if( text == m_model->CurentState() || text.isEmpty() )
         return;
    m_model->setCurentState( text );
}


void ddmMapsWidget::slotSetCurentCounty( const QString& text )
{
    if( text == m_model->CurentCounty() || text.isEmpty() )
         return;
    m_model->setCurentState( text );
}


void ddmMapsWidget::slotSetCurentState()
{
    QString text = m_leState->text();
    this->slotSetCurentState( text );

}

void ddmMapsWidget::slotSetCurentCounty()
{
    QString text = m_leCounty->text();
    this->slotSetCurentCounty( text );
}
