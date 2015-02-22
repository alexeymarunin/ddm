#include <QDoubleValidator>


#include "ddmFrictionCountyFilterWidget.h"
#include "ui_ddmfrictioncountyfilterwidget.h"


ddmFrictionCountyFilterWidget::ddmFrictionCountyFilterWidget( ddmFrictionCountyFilter* filter, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ddmFrictionCountyFilterWidget)
{
    ui->setupUi( this );
    this->setContentsMargins( 0,0,0,0 );
    ui->gridLayout_2->setContentsMargins( 0,0,0,0 );
    ui->gridLayout_3->setContentsMargins( 0,0,0,0 );
    ui->m_lbTitle->setWordWrap( true );
    ui->m_lbWarning->setWordWrap( true );
    ui->m_lbWarning->setText( "Некорректно заданны входные значения!" );
    ui->m_lbWarning->setStyleSheet( "QLabel { background-color : yellow; }");
    ui->m_lbWarning->hide();

    int decimals = 7;
    double minValue = 0.0, maxValue = 999.0;
    QDoubleValidator* leValidator = new QDoubleValidator( minValue, maxValue, decimals, this );
    leValidator->setNotation( QDoubleValidator::StandardNotation );
    ui->m_leFrom->setValidator( leValidator );
    ui->m_leTo->setValidator( leValidator );
    ui->m_leFrom->setText( "0,0" );
    ui->m_leTo->setText( "0,5" );
    m_filter = filter;
    installEvents();
}

ddmFrictionCountyFilterWidget::~ddmFrictionCountyFilterWidget()
{
    delete ui;
}


void ddmFrictionCountyFilterWidget::installEvents()
{
    // TODO: connect events here
    connect( ui->m_pbUpdateView, SIGNAL( pressed() ), this, SLOT( updateVisualize() ) );
    connect( ui->m_leFrom, SIGNAL( textEdited( const QString & ) ),  this, SLOT( slotVerifyData( const QString& ) ), Qt::UniqueConnection );
    connect( ui->m_leTo, SIGNAL( textEdited( const QString & ) ),  this, SLOT( slotVerifyData( const QString& ) ), Qt::UniqueConnection );
}


void ddmFrictionCountyFilterWidget::updateVisualize()
{
    // TODO: code here
}

void ddmFrictionCountyFilterWidget::slotVerifyData( const QString& text )
{
    if( ui->m_leFrom->text().isEmpty() || ui->m_leTo->text().isEmpty() )
    {
        ui->m_lbWarning->show();
        return;
    }

    // toDouble() не может преобразовать строку вида 1.2 ( нужна строка 1.2 )
    // QDoubleValidator дает вводить строки вида (1,2) (Qt bug?)
    // из-за этого меняем "," на "."
    QString mins, maxs;
    mins = ui->m_leFrom->text().replace( ",", "." );
    maxs = ui->m_leTo->text().replace( ",", "." );
    double min = mins.toDouble();
    double max = maxs.toDouble();
    if( min > max )
    {
        ui->m_lbWarning->show();
        return;
    }

    ui->m_lbWarning->hide();
}
