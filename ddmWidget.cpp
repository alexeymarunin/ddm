#include <QDir>
#include <QMessageBox>
#include <QPointF>
#include <QResizeEvent>
#include <QWebFrame>

#include "ui_ddmwidget.h"
#include "ddmWidget.h"
#include "ddmModel.h"
#include "ddmMapViewPage.h"

#include "ddmEmptyfilter.h"
#include "ddmFrictionCountyFilter.h"


ddmWidget::ddmWidget( ddmModel* model, QWidget* parent ) : QWidget( parent ),
    ui( new Ui::ddmWidget ),
    m_model( model )
{
    ui->setupUi( this );
    m_curWidget = NULL;
    ddmMapView* mapView = this->mapView();
    ddmMapViewPage* mapPage = new ddmMapViewPage;
    mapView->setPage( mapPage );

    // устанавливаем соединение с БД
    QString pathToDb = QObject::tr( "%1/ddm.sqlite" ).arg( QApplication::applicationDirPath() );
    if ( !QFile::exists( pathToDb ) )
    {
        // Если в текущем каталоге БД нет, то ищем в родительских
        pathToDb = QObject::tr( "%1/../../ddm.sqlite" ).arg( QApplication::applicationDirPath() );
    }
    this->model()->openDatabase( pathToDb );

    // устанавливаем страницу для отображения google maps
    QString pathToWeb = QObject::tr( "%1/index.html" ).arg( QApplication::applicationDirPath() );
    if ( !QFile::exists( pathToWeb ) )
    {
        // Если в текущем каталоге БД нет, то ищем в родительских
        pathToWeb = QObject::tr( "%1/../../index.html" ).arg( QApplication::applicationDirPath() );
    }
    QUrl url = QUrl::fromLocalFile( pathToWeb );
    this->mapView()->setUrl( url );

    fillFiltersList();
    installEvents();
    slotSetCurrentFilter( DDM_EMPTY_FILTER );
}


ddmWidget::~ddmWidget()
{
    delete ui;
    delete m_curWidget;
}


void ddmWidget::installEvents()
{
    connect( this->ui->m_map->page()->mainFrame(), SIGNAL( javaScriptWindowObjectCleared() ), this, SLOT( slotInjectModel() ) );
    connect( this->model(), SIGNAL( changedCoords( const QString& ,const QString& ) ), this, SLOT( changedCoords( const QString& ,const QString& ) ) );
    connect( ui->m_cmbFilter,  SIGNAL( activated ( int ) ), this,  SLOT( slotSetCurrentFilter( int ) ));
}


void ddmWidget::slotInjectModel()
{
    ui->m_map->page()->mainFrame()->addToJavaScriptWindowObject( "ddm_model", this->model() );
}


void ddmWidget::changedCoords( const QString& lat, const QString& lng )
{
    if( lat.isEmpty() || lng.isEmpty() )
        return;
    emit changedStatusBarCoords( lat, lng );
}


void ddmWidget::slotSetCurrentFilter( int index )
{
    if( m_curWidget )
    {
        ui->m_widgetContainerLayout->removeWidget( m_curWidget );
        m_curWidget->hide();
        m_curWidget = NULL;
    }

    if( index == DDM_EMPTY_FILTER )
    {
        m_curWidget = m_filters.at( DDM_EMPTY_FILTER )->getWidget();
        m_curWidget->setParent( this );
        m_curWidget->show();
        ui->m_widgetContainerLayout->addWidget( m_curWidget );

    }
    else if( index == DDM_FRICTION_COUNTY_FILTER )
    {
        m_curWidget = m_filters.at( DDM_FRICTION_COUNTY_FILTER )->getWidget();
        m_curWidget->setParent( this );
        m_curWidget->show();
        ui->m_widgetContainerLayout->addWidget( m_curWidget );
    }
    else if( index == DDM_MIGRATION_COUNTY_FILTER )
    {
       // TODO:
    }
}


void ddmWidget::fillFiltersList()
{
    ui->m_cmbFilter->addItem( "Пустой фильтр" );
    ui->m_cmbFilter->addItem( "Фильтр трения по графствам" );
    ui->m_cmbFilter->addItem( "Фильтр миграций из графства" );

    m_filters.append( new ddmEmptyFilter( model(), mapView() ) );
    m_filters.append( new ddmFrictionCountyFilter( model(), mapView() ) );
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


void ddmWidget::reload()
{
    if( ddmMapView* view = mapView() )
        view->reload();
}


void ddmWidget::increaseZoom()
{
    if( ddmMapView* view = mapView() )
        view->increaseZoomLevel();
}


void ddmWidget::decreaseZoom()
{
    if( ddmMapView* view = mapView() )
        view->decreaseZoomLevel();
}
