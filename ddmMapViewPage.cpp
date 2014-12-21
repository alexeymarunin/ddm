#include <QDebug>
#include "ddmMapViewPage.h"

ddmMapViewPage::ddmMapViewPage( QObject* parent ) : QWebPage( parent )
{
}

void ddmMapViewPage::javaScriptConsoleMessage( const QString& text, int line, const QString& source )
{
    QString message = QString( "%1, %2: %3" ).arg( source ).arg( line ).arg( text );
    qDebug() << message;
}

