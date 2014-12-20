#include "ui_ddmmapswidget.h"
#include "ddmmapswidget.h"
#include "ddmObjectModel.h"
#include "ddmContainer.h"

#include <QDir>
#include <QLineEdit>
#include <QMessageBox>
#include <QPointF>
#include <QResizeEvent>


ddmMapsWidget::ddmMapsWidget( QWidget *parent ) :
    QWidget(parent),
    ui(new Ui::ddmMapsWidget)
{
    ui->setupUi(this);

    m_leState = new QLineEdit( ui->m_cmbState );
    m_leCounty = new QLineEdit( ui->m_cmbCounty );

    // устанавливаем соединение с БД
    QString db_path = QObject::tr( "%1/ddm.sqlite" ).arg( QDir::current().path() );
    m_model = new ddmObjectModel();
    m_model->openDB( db_path );

    // устанавливаем страницу для отображения google maps
    QString path = QObject::tr( "%1/index.html" ).arg( QDir::current().path() );
    QUrl url = QUrl::fromLocalFile( path );
    ui->m_map->setUrl( url );

    // инициализация начальных значений
    QStringList states, counties;
    QString state, county;
    m_model->getStatesList( states );
    state = states.at( 0 );
    m_model->getCountiesList( counties, state );
    county = counties.at( 0 );
    slotSetCurentState( state );
    slotSetCurentCounty( county );

    ui->m_cmbState->setLineEdit(  m_leState );
    ui->m_cmbCounty->setLineEdit( m_leCounty );

    ui->m_cmbState->addItems( states );
    ui->m_cmbCounty->addItems( counties );

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
  connect( m_model, SIGNAL( updateCounty( const QString&, int ) ), this, SLOT( updateCounty( const QString&, int ) ));
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
    updateCountiesList( state );
    QString county = ui->m_cmbCounty->itemText( 0 );
    slotSetCurentCounty( county );
}


void ddmMapsWidget::updateCounty( const QString &county, int state_id )
{
    QPointF center = m_model->getCountyCenter( county );
    QString s_center = QObject::tr("%1,%2").arg( center.x() ).arg( center.y() );
    ddmContainer *county_info = new ddmContainer();
    int county_id = m_model->GetCurentCountyId();

    m_model->getCountyBoundary( county, county_info );
    county_info->setName( county );
    county_info->setCenter( s_center );
    county_info->setId( county_id );

    ui->m_map->setCenter( center );
    ui->m_map->drawPolygon( county_info );

    delete county_info;
}


void ddmMapsWidget::updateCountiesList( const QString &state )
{
    QStringList counties;
    m_model->getCountiesList( counties, state );
    ui->m_cmbCounty->clear();
    ui->m_cmbCounty->addItems( counties );
}

void ddmMapsWidget::resizeEvent( QResizeEvent *event )
{
    QSize size = event->size();
    ui->m_map->resize( size.width(), size.height() );
}
