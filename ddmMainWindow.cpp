#include <QFileDialog>
#include <QLabel>
#include <QList>
#include <QWebSettings>
#include <QDebug>
#include <QDesktopWidget>

#include "ddmApplication.h"
#include "ddmMainWindow.h"
#include "ui_ddmMainWindow.h"
#include "widgets/ddmCentralWidget.h"
#include "ddmSettings.h"

/**
 * Конструктор класса
 *
 * @param   parent Родитель (владелец) окна
 * @author  Цалко Т.В.
 * @since   2.0
 */
ddmMainWindow::ddmMainWindow( QWidget* parent ) : QMainWindow( parent )
{
    QWebSettings::globalSettings()->setAttribute( QWebSettings::DeveloperExtrasEnabled, true );

    Ui::ddmMainWindow* ui = new Ui::ddmMainWindow;
    ui->setupUi( this );

    this->setWindowFlags( this->windowFlags() | Qt::WindowMaximizeButtonHint );

    // Заголовок окна
    QString title = QString( "%1, v.%2" ).arg( QCoreApplication::applicationName() ).arg( QCoreApplication::applicationVersion() );
#ifdef QT_DEBUG
    title += QString( " [DEBUG MODE]" );
#endif
    this->setWindowTitle( title );

    // Панель статуса
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

    this->ui->menuData->menuAction()->setVisible( false );
    this->ui->openAction->setVisible( false );
    this->ui->createTableViewAction->setVisible( false );

    this->setupEvents();

    this->loadSettings();

}

/**
 * Настройка обработчиков главного окна
 *
 * @author  Цалко Т.В.
 * @since   2.0
 */
void ddmMainWindow::setupEvents()
{
    // добавляем обработку нажатия кнопки выход
    connect( this->ui->quitAction, SIGNAL( triggered() ), this, SLOT( slotQuit() ) );

    // обновляем html страницу
    this->ui->reloadAction->setShortcut( QKeySequence( tr( "F5" ) ) );
    connect( this->ui->reloadAction, SIGNAL( triggered() ), this, SLOT( slotReload() ) );
    connect( this->ui->increaseZoomAction, SIGNAL( triggered() ), this, SLOT( slotIncreaseZoom() ) );
    connect( this->ui->decreaseZoomAction, SIGNAL( triggered() ), this, SLOT( slotDecreaseZoom() ) );
}

/**
 * Загружает настройки окна
 *
 * @author  Марунин А.В.
 * @since   2.6
 */
void ddmMainWindow::loadSettings()
{
    ddmSettings* settings = ddmSettings::instance();

    QSize size = settings->value( "mainwindow/size", QSize( 800, 600 ) ).toSize();
    QPoint pos = settings->value( "mainwindow/position", QPoint( 0, 0 ) ).toPoint();
    this->move( pos );
    this->resize( size );
    bool maximized = settings->value( "mainwindow/maximized", true ).toBool();
    if ( maximized )
    {
        this->setWindowState( Qt::WindowMaximized );
    }
}

/**
 * Сохраняет настройки окна
 *
 * @author  Марунин А.В.
 * @since   2.6
 */
void ddmMainWindow::saveSettings()
{
    ddmSettings* settings = ddmSettings::instance();

    settings->setValue( "mainwindow/size", this->size() );
    settings->setValue( "mainwindow/position", this->pos() );
    settings->setValue( "mainwindow/maximized", this->isMaximized() );
}

/**
 * @brief ddmMainWindow::slotReload
 * @author  Цалко Т.В.
 * @since   2.0
 */
void ddmMainWindow::slotReload()
{
    ddmApp->centralWidget()->reload();
}

/**
 * @brief ddmMainWindow::slotIncreaseZoom
 * @author  Цалко Т.В.
 * @since   2.0
 */
void ddmMainWindow::slotIncreaseZoom()
{
    ddmApp->centralWidget()->increaseZoom();
}

/**
 *
 *
 * @author  Цалко Т.В.
 * @since   2.0
 */
void ddmMainWindow::slotDecreaseZoom()
{
    ddmApp->centralWidget()->decreaseZoom();
}

/**
 * Обработчик события перемещения курсора по карте
 *
 * @param   lat Долгота
 * @param   lng Широта
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmMainWindow::slotMapMouseMove( double lat, double lng )
{
    this->m_statusBarLat->setText( QObject::tr( "%1" ).arg( lat ) );
    this->m_statusBarLng->setText( QObject::tr( "%1" ).arg( lng ) );
}

/**
 *
 *
 * @author  Цалко Т.В.
 * @since   2.0
 */
void ddmMainWindow::slotQuit()
{
    this->close();
}

/**
 * Деструктор класса
 *
 * @author  Цалко Т.В.
 * @since   2.0
 */
ddmMainWindow::~ddmMainWindow()
{
    this->saveSettings();
}

