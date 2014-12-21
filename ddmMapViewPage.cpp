#include <QDebug>
#include <QWebFrame>
#include "ddmMapViewPage.h"

ddmMapViewPage::ddmMapViewPage( QObject* parent ) : QWebPage( parent )
{
    this->mainFrame()->setScrollBarPolicy( Qt::Vertical,   Qt::ScrollBarAlwaysOff );
    this->mainFrame()->setScrollBarPolicy( Qt::Horizontal, Qt::ScrollBarAlwaysOff );
}

void ddmMapViewPage::javaScriptConsoleMessage( const QString& text, int line, const QString& source )
{
    QString message = QString( "%1, %2: %3" ).arg( source ).arg( line ).arg( text );
    qDebug() << message;
}

