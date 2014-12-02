#include "ddmmapswidget.h"
#include "ui_ddmmapswidget.h"
#include "ddmObjectModel.h"

#include <QLineEdit>
#include <QMessageBox>


ddmMapsWidget::ddmMapsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ddmMapsWidget)
{
    ui->setupUi(this);

    m_leState = new QLineEdit( ui->m_cmbState );
    m_leCounty = new QLineEdit( ui->m_cmbCounty );

    m_model = new ddmObjectModel();

    ui->m_cmbState->setLineEdit(  m_leState );
    ui->m_cmbCounty->setLineEdit( m_leCounty );

    installEvents();
}

ddmMapsWidget::~ddmMapsWidget()
{
    delete m_model;
    delete ui;
}


void ddmMapsWidget::installEvents()
{
  connect( ui->m_cmbState,  SIGNAL( activated ( const QString& ) ), this,  SLOT( slotSetCurentState( const QString& ) ));
  connect( ui->m_cmbCounty, SIGNAL( activated ( const QString& ) ),  this, SLOT( slotSetCurentState( const QString& ) ) );

  connect( m_leState,  SIGNAL( editingFinished() ),  this,  SLOT(  slotSetCurentState() ));
  connect( m_leCounty, SIGNAL( editingFinished() ),  this,  SLOT(  slotSetCurentCounty() ) );

  connect( m_model, SIGNAL( updateState( const QString& ) ), this, SLOT( updateStateOnMap( const QString&  ) ),   Qt::UniqueConnection );
  connect( m_model, SIGNAL( updateCounty( const QString& ) ), this, SLOT( updateCountyOnMap( const QString&  ) ));

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
    m_model->setCurentCounty( text );
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


void ddmMapsWidget::updateStateOnMap( const QString &state )
{
    QString message = QObject::tr( "Выбран штат %1" ).arg( state );
    QMessageBox mb;
    mb.setText( message );
    mb.exec();
    // возможно должен вызываться как слот,
    // а не как простая функция
    updateCountiesList( state );
}


void ddmMapsWidget::updateCountyOnMap( const QString &county )
{
    QString message = QObject::tr( "Выбрано гравство %1" ).arg( county );
    QMessageBox mb;
    mb.setText( message );
    mb.exec();
}


void ddmMapsWidget::updateCountiesList( const QString &state )
{
    if( state == "Пенсильвания" )
    {
        ui->m_cmbCounty->addItem( QObject::tr( "Нортумберленд" ) );
        ui->m_cmbCounty->addItem( QObject::tr( "Канонсбург" ) );
    }
    else
    {
        ui->m_cmbCounty->clear();
    }
}
