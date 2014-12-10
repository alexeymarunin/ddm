#include "ddmmapswidget.h"
#include "ui_ddmmapswidget.h"
#include "ddmObjectModel.h"

#include <QDir>
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

    // страница для отображения google maps
    QString path = QObject::tr( "%1/index.html" ).arg( QDir::current().path() );
    QUrl url = QUrl::fromLocalFile( path );
    ui->m_map->setUrl( url );

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
  connect( ui->m_cmbCounty, SIGNAL( activated ( const QString& ) ),  this, SLOT( slotSetCurentCounty( const QString& ) ) );

  connect( m_model, SIGNAL( updateState( const QString& ) ), this, SLOT( updateState( const QString&  ) ),   Qt::UniqueConnection );
  connect( m_model, SIGNAL( updateCounty( const QString& ) ), this, SLOT( updateCounty( const QString&  ) ));

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


void ddmMapsWidget::updateState( const QString &state )
{
    QString message = QObject::tr( "Выбран штат %1" ).arg( state );
    QMessageBox mb;
    mb.setText( message );
    mb.exec();
    // возможно должен вызываться как слот,
    // а не как простая функция
    updateCountiesList( state );
    QString county = ui->m_cmbCounty->itemText( 0 );
    slotSetCurentCounty( county );
}


void ddmMapsWidget::updateCounty( const QString &county )
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
