#include <QFileDialog>
#include <QWebSettings>
#include <QLabel>
#include <QList>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ddmWidget.h"
#include "ddmTableView.h"


MainWindow::MainWindow( QWidget* parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    QWebSettings::globalSettings()->setAttribute( QWebSettings::DeveloperExtrasEnabled, true );

    m_viewsVector.clear();
    ui->setupUi( this );
    m_statusBarLat = new QLabel( this );
    m_statusBarLng = new QLabel( this );
    m_statusBarLat->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    m_statusBarLng->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    ui->statusBar->addPermanentWidget( m_statusBarLat, 1 );
    ui->statusBar->addPermanentWidget( m_statusBarLng, 1 );
    setWindowFlags( windowFlags() | Qt::WindowMaximizeButtonHint );
    m_viewMapWidget = new ddmWidget( this->model(), this );
    setCentralWidget( m_viewMapWidget );
    InstallEvents();

}

ddmModel* MainWindow::model() const
{
    return const_cast<ddmModel*>( &this->m_model );
}

MainWindow::~MainWindow()
{
    delete m_statusBarLat;
    delete m_statusBarLng;
    delete ui;
    m_viewsVector.clear();
}


void MainWindow::InstallEvents()
{
    // добавляем обработку нажатия кнопки открыть
    connect( ui->openAction, SIGNAL( triggered() ),  this, SLOT( slotOpen() ) );

    // добавляем обработку нажатия кнопки выход
    connect( ui->quitAction, SIGNAL( triggered() ), this, SLOT( slotQuit() ) );

    // обновляем html страницу
    connect( ui->reloadAction, SIGNAL( triggered() ), this, SLOT( slotReload() ) );

#ifdef QT_DEBUG
    ui->reloadAction->setDisabled( true );
#endif

    connect( ui->increaseZoomAction, SIGNAL( triggered() ), this, SLOT( slotIncreaseZoom() ) );
    connect( ui->decreaseZoomAction, SIGNAL( triggered() ), this, SLOT( slotDecreaseZoom() ) );

    connect( m_viewMapWidget, SIGNAL( changedStatusBarCoords( const QString& , const QString& ) ), this, SLOT( changedStatusBarCoords( const QString&, const QString& ) ) );

    connect( ui->createTableViewAction, SIGNAL( triggered() ), this, SLOT( createTableView() ) );

}


void MainWindow::slotQuit()
{
    this->close();
}


void MainWindow::slotOpen()
{
    QString caption = "Открыть файл с исходными данными";
    QString fileName = QFileDialog::getOpenFileName(this,  caption,"",
                                                    QObject::tr("Data Files(*.xls)"));
    if( fileName.isEmpty() )
        return;
}


void MainWindow::slotReload()
{
    if( m_viewMapWidget )
        m_viewMapWidget->reload();
}


void MainWindow::slotIncreaseZoom()
{
    m_viewMapWidget->increaseZoom();
}


void MainWindow::slotDecreaseZoom()
{
    m_viewMapWidget->decreaseZoom();
}


void MainWindow::changedStatusBarCoords( const QString &lat, const QString &lng )
{
    if( m_statusBarLat == NULL || m_statusBarLng == NULL )
        return;
    m_statusBarLat->setText( QObject::tr( "Широта: %1" ).arg( lat ) );
    m_statusBarLng->setText( QObject::tr( "Долгота: %1" ).arg( lng ) );
}


void MainWindow::createTableView()
{
  ddmTableView* view = NULL;
  int size = m_viewsVector.size();
  for( int i = 0; i < size; ++i )
  {
      if(  m_viewsVector[i]->isHidden() )
          view = m_viewsVector[i];
  }

  if( view == NULL )
  {
      view = new ddmTableView( this );
      m_viewsVector.append( view );
  }

  view->show();
}
