#ifndef DDM_MAP_VIEW_PAGE_H
#define DDM_MAP_VIEW_PAGE_H

#include <QWebPage>

/**
 * Класс ddmMapViewPage описывает веб-страницу, на которой отображается Google Map
 * Основное назначение - перенаправление вывода сообщений консоли JavaScript в консоль приложения
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
class ddmMapViewPage : public QWebPage
{
    Q_OBJECT

public:

    ddmMapViewPage( QObject* parent = 0 );

    virtual ~ddmMapViewPage();

protected:

    void javaScriptConsoleMessage( const QString& text, int line, const QString& source );

};

#endif // DDM_MAP_VIEW_PAGE_H
