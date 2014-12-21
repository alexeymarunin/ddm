#include <QDir>
#include <QLineEdit>
#include <QMessageBox>
#include <QPointF>
#include <QResizeEvent>
#include <QWebFrame>

#include "ui_ddmwidget.h"
#include "ddmWidget.h"
#include "ddmModel.h"
#include "ddmMapViewPage.h"


ddmWidget::ddmWidget( ddmModel* model, QWidget* parent ) : QWidget( parent ),
    ui( new Ui::ddmWidget ),
    m_model( model )
{
    ui->setupUi( this );

    ddmMapView* mapView = this->mapView();
    ddmMapViewPage* mapPage = new ddmMapViewPage;
    mapView->setPage( mapPage );

    m_leState = new QLineEdit( ui->m_cmbState );
    m_leCounty = new QLineEdit( ui->m_cmbCounty );

    // устанавливаем соединение с БД
    QString pathToDb = QObject::tr( "%1/ddm.sqlite" ).arg( QDir::current().path() );
    this->model()->openDatabase( pathToDb );

    // устанавливаем страницу для отображения google maps
    QString pathToWeb = QObject::tr( "%1/index.html" ).arg( QDir::current().path() );
    QUrl url = QUrl::fromLocalFile( pathToWeb );
    this->mapView()->setUrl( url );

    // инициализация начальных значений
    ui->m_cmbState->setLineEdit(  m_leState );
    ui->m_cmbCounty->setLineEdit( m_leCounty );

    QStringList states, counties;
    states = this->model()->stateNames();
    counties = this->model()->countyNames();
    ui->m_cmbState->addItems( states );
    ui->m_cmbCounty->addItems( counties );

    this->slotSetCurrentState( this->model()->currentState()->geographicName() );
    this->slotSetCurrentCounty( this->model()->currentCounty()->geographicName() );

    installEvents();


}

void ddmWidget::installEvents()
{
    connect( ui->m_cmbState,  SIGNAL( activated ( const QString& ) ), this,  SLOT( slotSetCurrentState( const QString& ) ));
    connect( ui->m_cmbCounty, SIGNAL( activated ( const QString& ) ), this, SLOT( slotSetCurrentCounty( const QString& ) ) );

    connect( this->model(), SIGNAL( changedState( ddmState* ) ), this, SLOT( changedState( ddmState* ) ), Qt::UniqueConnection );
    connect( this->model(), SIGNAL( changedCounty( ddmCounty* ) ), this, SLOT( changedCounty( ddmCounty* ) ) );
    connect( this->ui->m_map->page()->mainFrame(), SIGNAL( javaScriptWindowObjectCleared() ), this, SLOT( slotInjectModel() ) );
}


void ddmWidget::slotInjectModel()
{
    ui->m_map->page()->mainFrame()->addToJavaScriptWindowObject( "ddm_model", this->model() );
}

void ddmWidget::slotSetCurrentState( const QString& stateName )
{
    this->model()->setCurrentState( stateName );
    this->updateCountiesList();
}


void ddmWidget::slotSetCurrentCounty( const QString& countyName )
{
    this->model()->setCurrentCounty( countyName );
}


void ddmWidget::changedState( ddmState* state )
{
    this->slotSetCurrentState( state->geographicName() );
    ui->m_cmbState->setEditText( this->model()->currentState()->geographicName() );
}

void ddmWidget::changedCounty( ddmCounty* county )
{
    this->slotSetCurrentCounty( county->geographicName() );
    this->mapView()->selectCounty( county->id() );
}


void ddmWidget::updateCountiesList()
{
    QStringList counties( this->model()->countyNames() );
    ui->m_cmbCounty->clear();
    ui->m_cmbCounty->addItems( counties );
    ui->m_cmbCounty->setEditText( this->model()->currentCounty()->geographicName() );
}

void ddmWidget::resizeEvent( QResizeEvent *event )
{
    QSize size = event->size();
    ui->m_map->resize( size.width(), size.height() );
}


void ddmWidget::setModel( ddmModel* model )
{
    this->m_model = model;
}

ddmModel* ddmWidget::model() const
{
    return this->m_model;
}

ddmMapView* ddmWidget::mapView() const
{
    return this->ui->m_map;
}

ddmWidget::~ddmWidget()
{
    delete ui;
}

