#include <QFileDialog>
#include <QLabel>
#include <QList>
#include <QWebSettings>

#include "ddmApplication.h"
#include "ddmMainWindow.h"
#include "ui_ddmMainWindow.h"
#include "widgets/ddmCentralWidget.h"
// #include "ddmTableView.h"


ddmMainWindow::ddmMainWindow( QWidget* parent ) : QMainWindow( parent )
{
    QWebSettings::globalSettings()->setAttribute( QWebSettings::DeveloperExtrasEnabled, true );

    Ui::ddmMainWindow* ui = new Ui::ddmMainWindow;

    ui->setupUi( this );
    this->setWindowFlags( this->windowFlags() | Qt::WindowMaximizeButtonHint );

    //m_viewsVector.clear();

    this->m_statusBarLat = new QLabel( this );
    this->m_statusBarLat->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    this->m_statusBarLat->setFixedWidth( 70 );
    this->m_statusBarLat->setAlignment( Qt::AlignCenter );
    ui->statusBar->addPermanentWidget( m_statusBarLat );

    this->m_statusBarLng = new QLabel( this );
    this->m_statusBarLng->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    this->m_statusBarLng->setFixedWidth( 70 );
    this->m_statusBarLng->setAlignment( Qt::AlignCenter );
    ui->statusBar->addPermanentWidget( m_statusBarLng );

    this->ui = ui;
    InstallEvents();

}

void ddmMainWindow::InstallEvents()
{
/*
    // добавляем обработку нажатия кнопки открыть
    connect( ui->openAction, SIGNAL( triggered() ),  this, SLOT( slotOpen() ) );

    connect( ui->increaseZoomAction, SIGNAL( triggered() ), this, SLOT( slotIncreaseZoom() ) );
    connect( ui->decreaseZoomAction, SIGNAL( triggered() ), this, SLOT( slotDecreaseZoom() ) );

    connect( m_centralWidget, SIGNAL( changedStatusBarCoords( const QString& , const QString& ) ), this, SLOT( changedStatusBarCoords( const QString&, const QString& ) ) );

    connect( ui->createTableViewAction, SIGNAL( triggered() ), this, SLOT( createTableView() ) );
*/
    // добавляем обработку нажатия кнопки выход
    connect( ui->quitAction, SIGNAL( triggered() ), this, SLOT( slotQuit() ) );

    // обновляем html страницу
    connect( ui->reloadAction, SIGNAL( triggered() ), this, SLOT( slotReload() ) );

#ifdef QT_DEBUG
    //ui->reloadAction->setDisabled( true );
#endif

}


void ddmMainWindow::slotQuit()
{
    this->close();
}

/*
void ddmMainWindow::slotOpen()
{
    QString caption = "Открыть файл с исходными данными";
    QString fileName = QFileDialog::getOpenFileName(this,  caption,"",
                                                    QObject::tr("Data Files(*.xls)"));
    if( fileName.isEmpty() )
        return;
}
*/

void ddmMainWindow::slotReload()
{
    ddmApp->centralWidget()->reload();
}


void ddmMainWindow::slotIncreaseZoom()
{
    ddmApp->centralWidget()->increaseZoom();
}


void ddmMainWindow::slotDecreaseZoom()
{
    ddmApp->centralWidget()->decreaseZoom();
}


void ddmMainWindow::slotMapMouseMove( double lat, double lng )
{
    if ( this->m_statusBarLat && this->m_statusBarLng  )
    {
        this->m_statusBarLat->setText( QObject::tr( "%1" ).arg( lat ) );
        this->m_statusBarLng->setText( QObject::tr( "%1" ).arg( lng ) );
    }
}

/*
void ddmMainWindow::createTableView()
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

*/

/**
 * @brief ddmMainWindow::~ddmMainWindow
 */
ddmMainWindow::~ddmMainWindow()
{
    //m_viewsVector.clear();
}

