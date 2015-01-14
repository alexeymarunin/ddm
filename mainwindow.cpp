#include <QFileDialog>
#include <QWebSettings>
#include <QLabel>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ddmWidget.h"

MainWindow::MainWindow( QWidget* parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    QWebSettings::globalSettings()->setAttribute( QWebSettings::DeveloperExtrasEnabled, true );

    ui->setupUi( this );
    m_statusBarMessage = new QLabel( this );
    ui->statusBar->addWidget( m_statusBarMessage );
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
    delete m_statusBarMessage;
    delete ui;
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
    if( m_statusBarMessage == NULL )
        return;
    m_statusBarMessage->setText( QObject::tr( "Широта: %1 Долгота: %2" ).arg( lat ).arg( lng ) );
}
