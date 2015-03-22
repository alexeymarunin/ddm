#include <QApplication>
#include <QDir>
#include <QImageReader>
#include <QTextEdit>
#include <QMenu>
#include <QTextDocumentFragment>
#include <QTime>

#include "ddmInfoLogger.h"



ddmInfoLogger* ddmInfoLogger::m_instance = NULL;

ddmInfoLogger::ddmInfoLogger()
{
    m_outputWindow = NULL;
}



ddmInfoLogger& ddmInfoLogger::instance()
{
    if ( !ddmInfoLogger::m_instance )
        ddmInfoLogger::m_instance = new ddmInfoLogger();

    return *ddmInfoLogger::m_instance;
}


ddmInfoLogger::~ddmInfoLogger()
{
     m_outputWindow = NULL;
}


void ddmInfoLogger::writeDebug( const QString &text )
{
   QString imageFilePath = QObject::tr( "%1/bug.png" ).arg( getCurrentPathToIcon() );
   insertImage( imageFilePath );

   QString time = getCurrentTime();
   QString message;
   if( text.endsWith( "\n" ) || text.endsWith( "\n\r" ) )
      message =  QObject::tr( "%1:%2" ).arg( time ).arg( text );
   else
      message =  QObject::tr( "%1:%2\n" ).arg( time ).arg( text );
   m_outputWindow->insertPlainText( message );
}


void ddmInfoLogger::writeInfo( const QString &text )
{
    QString imageFilePath = QObject::tr( "%1/info.png" ).arg( getCurrentPathToIcon() );
    insertImage( imageFilePath );
    QString time = getCurrentTime();
    QString message;
    if( text.endsWith( "\n" ) || text.endsWith( "\n\r" ) )
       message =  QObject::tr( "%1:%2" ).arg( time ).arg( text );
    else
       message =  QObject::tr( "%1:%2\n" ).arg( time ).arg( text );
    m_outputWindow->insertPlainText( message );
}


void ddmInfoLogger::writeWarning( const QString &text )
{
    QString imageFilePath = QObject::tr( "%1/warning.png" ).arg( getCurrentPathToIcon() );
    insertImage( imageFilePath );
    QString time = getCurrentTime();
    QString message;
    if( text.endsWith( "\n" ) || text.endsWith( "\n\r" ) )
       message =  QObject::tr( "%1:%2" ).arg( time ).arg( text );
    else
       message =  QObject::tr( "%1:%2\n" ).arg( time ).arg( text );
    m_outputWindow->insertPlainText( message );
}


void ddmInfoLogger::writeError( const QString &text )
{
    QString imageFilePath = QObject::tr( "%1/error.png" ).arg( getCurrentPathToIcon() );
    insertImage( imageFilePath );
    QString time = getCurrentTime();
    QString message;
    if( text.endsWith( "\n" ) || text.endsWith( "\n\r" ) )
       message =  QObject::tr( "%1:%2" ).arg( time ).arg( text );
    else
       message =  QObject::tr( "%1:%2\n" ).arg( time ).arg( text );
    m_outputWindow->insertPlainText( message );
}


void ddmInfoLogger::setOutputWidget(QTextEdit *outputWidget )
{
    m_outputWindow = outputWidget;
    if( m_outputWindow )
    {
        m_outputWindow->setReadOnly( true );
        m_outputWindow->setFont( QFont( "Time New Roman", 10 ) );
    }
}


QString ddmInfoLogger::getCurrentTime()
{
    QTime time = QTime::currentTime();
    return time.toString();
}


QString ddmInfoLogger::getCurrentPathToIcon()
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


void ddmInfoLogger::insertImage( const QString& fullFileName )
{
    QTextDocumentFragment fragment = QTextDocumentFragment::fromHtml( QObject::tr( "<img src='%1'>" ).arg( fullFileName ) );
    m_outputWindow->textCursor().insertFragment( fragment );
}
