#include <QDebug>
#include <QWebFrame>
#include "ddmMapViewPage.h"

/**
 * Конструктор класса
 *
 * @param   parent Родитель (владелец)
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmMapViewPage::ddmMapViewPage( QObject* parent ) : QWebPage( parent )
{
    this->mainFrame()->setScrollBarPolicy( Qt::Vertical,   Qt::ScrollBarAlwaysOff );
    this->mainFrame()->setScrollBarPolicy( Qt::Horizontal, Qt::ScrollBarAlwaysOff );
}

/**
 * Обработчик события вывода в консоль JavaScript
 *
 * Перенаправляет вывод с JS-консоли в консоль приложения
 *
 * @param   text Текст, выводимый в консоль
 * @param   line Номер строки
 * @param   source Имя файла
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmMapViewPage::javaScriptConsoleMessage( const QString& text, int line, const QString& source )
{
    QString message = QString( "%1, %2: %3" ).arg( source ).arg( line ).arg( text );
    if ( source.isEmpty() ) message = text;
    qDebug() << message;
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmMapViewPage::~ddmMapViewPage()
{
}
