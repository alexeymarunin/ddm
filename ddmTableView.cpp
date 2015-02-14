#include <QResizeEvent>
#include <QMessageBox>

#include "ddmTableView.h"
#include "ui_ddmtableview.h"


ddmTableView::ddmTableView( QWidget *parent ):
    QDialog(parent),
    ui(new Ui::ddmTableView)
{
    ui->setupUi(this);
    this->setWindowTitle( QObject::tr( "Таблица отображения данных" ) );

    m_model = new ddmTableViewModel();
    QHeaderView *horizontalHeader = ui->tableView->horizontalHeader();
    horizontalHeader->sectionResizeMode( QHeaderView::Fixed );
    int colums = 3, diff = 10;
    int headerSize = ( this->size().width()/colums ) - diff;
    horizontalHeader->setDefaultSectionSize( headerSize );
    ui->tableView->horizontalHeader()->setSectionsMovable( true );
    ui->tableView->horizontalHeader()->resizeSections( QHeaderView::Stretch );
    ui->tableView->setModel( m_model );
    installEventFilter();
}

ddmTableView::~ddmTableView()
{
    delete ui;
}


void ddmTableView::resizeEvent( QResizeEvent *event )
{
    int colums = 3, diff = 10;
    int headerSize = ( event->size().width()/colums ) - diff;
    QHeaderView *horizontalHeader = ui->tableView->horizontalHeader();
    horizontalHeader->setDefaultSectionSize( headerSize );
}


void ddmTableView::installEventFilter()
{
    connect( ui->pbImportData, SIGNAL( clicked() ), this, SLOT( slotImportData() ) );
    connect( ui->pbExportData, SIGNAL( clicked() ), this, SLOT( slotExportData() ) );
    connect( ui->pbPrint, SIGNAL( clicked() ), this, SLOT( slotPrint() ) );

}


void ddmTableView::slotImportData()
{
    QMessageBox mb;
    mb.setText( QObject::tr( "Слот импорта данных" ) );
    mb.exec();
}


void ddmTableView::slotExportData()
{
    QMessageBox mb;
    mb.setText( QObject::tr( "Слот экспорта данных" ) );
    mb.exec();
}


void ddmTableView::slotPrint()
{
    QMessageBox mb;
    mb.setText( QObject::tr( "Слот печати" ) );
    mb.exec();
}
