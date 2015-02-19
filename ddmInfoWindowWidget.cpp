#include <QDir>
#include <QImageReader>
#include <QMenu>
#include <QTextDocumentFragment>
#include <QTime>

#include "ddmInfoWindowWidget.h"
#include "ui_ddminfowindowwidget.h"


ddmInfoWindowWidget::ddmInfoWindowWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ddmInfoWindowWidget)
{
    ui->setupUi(this);
    ui->m_ptInfoWindow->setReadOnly( true );
    ui->m_ptInfoWindow->setFont ( QFont( "Time New Roman", 10 ) );

    ui->m_ptInfoWindow->setContextMenuPolicy( Qt::CustomContextMenu );
    connect( ui->m_ptInfoWindow , SIGNAL(customContextMenuRequested( const QPoint & )),
            this, SLOT(ShowContextMenu( const QPoint &) ) );
}

ddmInfoWindowWidget::~ddmInfoWindowWidget()
{
    delete ui;
}


void ddmInfoWindowWidget::writeDebug( const QString &text )
{
   QString imageFilePath = QObject::tr( "%1/bug.png" ).arg( getCurrentPathToIcon() );
   insertImage( imageFilePath );

   QString time = getCurrentTime();
   QString message;
   if( text.endsWith( "\n" ) || text.endsWith( "\n\r" ) )
      message =  QObject::tr( "%1:%2" ).arg( time ).arg( text );
   else
      message =  QObject::tr( "%1:%2\n" ).arg( time ).arg( text );
   ui->m_ptInfoWindow->insertPlainText( message );
}


void ddmInfoWindowWidget::writeInfo( const QString &text )
{
    QString imageFilePath = QObject::tr( "%1/info.png" ).arg( getCurrentPathToIcon() );
    insertImage( imageFilePath );
    QString time = getCurrentTime();
    QString message;
    if( text.endsWith( "\n" ) || text.endsWith( "\n\r" ) )
       message =  QObject::tr( "%1:%2" ).arg( time ).arg( text );
    else
       message =  QObject::tr( "%1:%2\n" ).arg( time ).arg( text );
    ui->m_ptInfoWindow->insertPlainText( message );
}


void ddmInfoWindowWidget::writeWarning( const QString &text )
{
    QString imageFilePath = QObject::tr( "%1/warning.png" ).arg( getCurrentPathToIcon() );
    insertImage( imageFilePath );
    QString time = getCurrentTime();
    QString message;
    if( text.endsWith( "\n" ) || text.endsWith( "\n\r" ) )
       message =  QObject::tr( "%1:%2" ).arg( time ).arg( text );
    else
       message =  QObject::tr( "%1:%2\n" ).arg( time ).arg( text );
    ui->m_ptInfoWindow->insertPlainText( message );
}


void ddmInfoWindowWidget::writeError( const QString &text )
{
    QString imageFilePath = QObject::tr( "%1/error.png" ).arg( getCurrentPathToIcon() );
    insertImage( imageFilePath );
    QString time = getCurrentTime();
    QString message;
    if( text.endsWith( "\n" ) || text.endsWith( "\n\r" ) )
       message =  QObject::tr( "%1:%2" ).arg( time ).arg( text );
    else
       message =  QObject::tr( "%1:%2\n" ).arg( time ).arg( text );
    ui->m_ptInfoWindow->insertPlainText( message );
}


void ddmInfoWindowWidget::slotClearText()
{
    ui->m_ptInfoWindow->clear();
}


void ddmInfoWindowWidget::ShowContextMenu( const QPoint& point )
{
    QMenu contextMenu(tr( "Context menu" ), this );
    ui->m_clearTextAction->setParent( this );
    connect( ui->m_clearTextAction, SIGNAL( triggered() ), this,
             SLOT( slotClearText() ), Qt::UniqueConnection );
    contextMenu.addAction( ui->m_clearTextAction );
    contextMenu.exec( this->mapToGlobal( point ) );

}


QString ddmInfoWindowWidget::getCurrentTime()
{
    QTime time = QTime::currentTime();
    return time.toString();
}


QString ddmInfoWindowWidget::getCurrentPathToIcon()
{
    QString appPath = QApplication::applicationDirPath();
    QString iconDirPath = QObject::tr( "%1/icons" ).arg( appPath );
    QDir iconDir( iconDirPath );
    if( iconDir.exists() )
        return iconDirPath;

    iconDirPath = QObject::tr( "%1/../../icons" ).arg( appPath );
    iconDir.setPath( appPath );
    if( iconDir.exists() )
        return iconDirPath;
    return "";
}


void ddmInfoWindowWidget::insertImage( const QString& fullFileName )
{
    QTextDocumentFragment fragment = QTextDocumentFragment::fromHtml( QObject::tr( "<img src='%1'>" ).arg( fullFileName ) );
    ui->m_ptInfoWindow->textCursor().insertFragment( fragment );
}
