#ifndef DDM_MAP_VIEW_PAGE_H
#define DDM_MAP_VIEW_PAGE_H

#include <QWebPage>

class ddmMapViewPage : public QWebPage
{
    Q_OBJECT
public:
    explicit ddmMapViewPage( QObject* parent = 0 );

signals:

public slots:

protected:
    void javaScriptConsoleMessage( const QString& text, int line, const QString& source );
};

#endif // DDM_MAP_VIEW_PAGE_H
