#include <QApplication>
#include <QDir>
#include <QImageReader>
#include <QMenu>
#include <QTextDocumentFragment>
#include <QTime>

#include "ddmInfoLogger.h"


ddmInfoLogger* ddmInfoLogger::m_instance = NULL;


ddmInfoLogger::ddmInfoLogger() : QObject(),
    m_outputWindow( NULL )
{
}

ddmInfoLogger& ddmInfoLogger::instance()
{
    if ( !ddmInfoLogger::m_instance )
    {
        ddmInfoLogger::m_instance = new ddmInfoLogger();
    }

    return *ddmInfoLogger::m_instance;
}

void ddmInfoLogger::writeDebug( const QString &text )
{
    this->writeLog( "bug", text );
}

void ddmInfoLogger::writeInfo( const QString &text )
{
    this->writeLog( "info", text );
}

void ddmInfoLogger::writeWarning( const QString &text )
{
    this->writeLog( "warning", text );
}

void ddmInfoLogger::writeError( const QString &text )
{
    this->writeLog( "error", text );
}

void ddmInfoLogger::writeLog( const QString& icon, const QString& text )
{
    QString imageFilePath = QObject::tr( "%1/%2.png" ).arg( this->getCurrentPathToIcon() ).arg( icon );
    this->insertImage( imageFilePath );
    QString time = this->getCurrentTime();
    QString message;
    if ( text.endsWith( "\n" ) || text.endsWith( "\n\r" ) )
    {
        message =  QObject::tr( "%1: %2" ).arg( time ).arg( text );
    }
    else
    {
        message =  QObject::tr( "%1: %2\n" ).arg( time ).arg( text );
    }
    this->outputWindow()->insertPlainText( message );
}

void ddmInfoLogger::setOutputWindow( QTextEdit* outputWidget )
{
    if ( this->m_outputWindow != outputWidget )
    {
        this->m_outputWindow = outputWidget;
        if ( outputWidget )
        {
            this->m_outputWindow->setReadOnly( true );
            this->setParent( outputWidget );
            //m_outputWindow->setFont( QFont( "Time New Roman", 10 ) ); // ???
        }
    }
}

QTextEdit* ddmInfoLogger::outputWindow() const
{
    return const_cast<ddmInfoLogger*>( this )->m_outputWindow;
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
    if ( iconDir.exists() )
    {
        return iconDirPath;
    }

    iconDirPath = QObject::tr( "%1/../../icons" ).arg( appPath );
    iconDir.setPath( appPath );
    if( iconDir.exists() )
    {
        return iconDirPath;
    }

    return "";
}

void ddmInfoLogger::insertImage( const QString& fullFileName )
{
    QTextDocumentFragment fragment = QTextDocumentFragment::fromHtml( QObject::tr( "<img src='%1'/> " ).arg( fullFileName ) );
    this->outputWindow()->textCursor().insertFragment( fragment );
}

ddmInfoLogger::~ddmInfoLogger()
{
     this->m_outputWindow = NULL;
}

