#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ddmmapswidget.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi( this );
    setWindowFlags( windowFlags() | Qt::WindowMaximizeButtonHint );
    CreateMenuEvents();
    m_viewMapWidget = new ddmMapsWidget( this );
    setCentralWidget( m_viewMapWidget );

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::CreateMenuEvents()
{
    // добавляем обработку нажатия кнопки открыть
    connect( ui->openAction, SIGNAL( triggered() ),  this, SLOT( slotOpen() ) );

    // добавляем обработку нажатия кнопки выход
    connect( ui->quitAction, SIGNAL( triggered() ), this, SLOT( slotQuit() ));
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
