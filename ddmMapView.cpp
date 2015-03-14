#include <QtGlobal>
#include <QDir>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QWebFrame>
#include <QWebElement>

#include "filters/ddmFilter.h"
#include "ddmMapViewPage.h"
#include "ddmMainWindow.h"
#include "ddmApplication.h"
#include "widgets/ddmCentralWidget.h"
#include "ddmMapView.h"

/**
 * Имя файла HTML-страницы, в которую загружается карта Google Map
 **/
#define DDM_INDEX_PAGE "index.html"


/**
 * Конструктор класса
 *
 * @param   filter Фильтр, к которому относится браузер
 * @param   parent Виджет, которому принадлежит браузер.
 *          Если не задан, то владельцем становится центральный виджет приложения
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmMapView::ddmMapView( ddmFilter* filter, QWidget* parent ) : QWebView( parent ),
    m_filter( filter ), m_pendingRequests( 0 )
{
    this->m_mapLayout = ddmApp->centralWidget()->mapLayout();

    this->setContentsMargins( 0, 0, 0, 0 );
    this->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
    this->setMinimumSize( 500, 300 );

    this->m_manager = new QNetworkAccessManager( this );

    ddmMapViewPage* mapPage = new ddmMapViewPage( this );
    this->setPage( mapPage );

    QWebFrame* pageFrame = mapPage->mainFrame();

    // отключаем полосы прокрутки
    pageFrame->setScrollBarPolicy( Qt::Vertical,   Qt::ScrollBarAlwaysOff );
    pageFrame->setScrollBarPolicy( Qt::Horizontal, Qt::ScrollBarAlwaysOff );


    // Ищем, где находится index.html
    QString currentDir = QDir::current().absolutePath();
    QStringList dirs;
    dirs.append( currentDir );
    dirs.append( QObject::tr( "%1/../.." ).arg( currentDir ) );
    dirs.append( QObject::tr( "%1/.." ).arg( currentDir ) );

    QString pathIndex;
    foreach ( QString dir, dirs )
    {
        QString path = QDir( QObject::tr( "%1/%2" ).arg( dir ).arg( DDM_INDEX_PAGE ) ).absolutePath();
        if ( QFile::exists( path ) )
        {
            // qDebug() << "Found page " << path;
            pathIndex = path;
            break;
        }
    }

    Q_ASSERT( !pathIndex.isEmpty() );

    QUrl url = QUrl::fromLocalFile( pathIndex );
    this->setUrl( url );

    QObject::connect( this, SIGNAL( loaded() ), this, SLOT( slotLoaded() ) );
    QObject::connect( pageFrame, SIGNAL( javaScriptWindowObjectCleared() ), this, SLOT( slotJavaScriptWindowObjectCleared() ) );
    QObject::connect( this->m_manager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( slotReplyFinished(QNetworkReply*) ) );

    // Отслеживаем движение курсора мышки по карте
    ddmMainWindow* mainWindow = ddmApp->mainWindow();
    connect( this, SIGNAL( mousemove(double,double) ), mainWindow, SLOT( slotMapMouseMove(double,double) ) );

}

/**
 * Возвращает флаг, определяющий, загружена ли карта
 *
 * Флаг сбрасывается при вызове метода reload()
 *
 * @return true, если карта полностью загружена
 * @author  Марунин А.В.
 * @since   2.0
 */
bool ddmMapView::mapReady() const
{
    return this->m_mapReady;
}

/**
 * Обработчик события, срабатывающего после обновления окна браузера
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmMapView::slotJavaScriptWindowObjectCleared()
{
    this->m_mapReady = false;
    this->addToJavaScriptWindowObject( "ddmMapView", this );

    Q_EMIT javaScriptWindowObjectCleared(); // передаем дальше по цепочке
}

/**
 * Обработчик события, срабатывающего после полной загрузки карты
 *
 * @author  Марунин А.В.
 * @since   2.2
 */
void ddmMapView::slotLoaded()
{
    this->m_mapReady = true;
}

/**
 * Увеличить масштаб карты
 *
 * @author  Цалко Т.В.
 * @since   2.0
 */
void ddmMapView::increaseZoomLevel()
{
    this->evaluateJavaScript( QObject::tr( "ddmMap.zoom_in();" ) );
}

/**
 * Уменьшить масштаб карты
 *
 * @author  Цалко Т.В.
 * @since   2.0
 */
void ddmMapView::decreaseZoomLevel()
{
    this->evaluateJavaScript( QObject::tr( "ddmMap.zoom_out();" ) );
}

/**
 *
 * Устанавает центр видимой области карты
 *
 * @param center
 * @author  Марунин А.В.
 * @since   2.1
 */
void ddmMapView::setCenter( const QVariantMap& center )
{
    double x = center["x"].toDouble();
    double y = center["y"].toDouble();
    this->setCenter( x, y );
}

/**
 * Устанавает центр видимой области карты
 *
 * @param x
 * @param y
 * @author  Марунин А.В.
 * @since   2.1
 */
void ddmMapView::setCenter( double x, double y )
{
    if ( this->mapReady() )
    {
        this->evaluateJavaScript( QString( "window.ddmMap.panTo( %1, %2 );" ).arg( x ).arg( y ) );
    }
}

/**
 * Устанаваливает маркер на карте
 *
 * @param   point Объект типа QVariantMap, в котором записаны координаты маркера
 * @author  Марунин А.В.
 * @since   2.1
 */
void ddmMapView::setMarker( const QVariantMap& point )
{
    double x = point["x"].toDouble();
    double y = point["y"].toDouble();
    this->setMarker( x, y );
}

/**
 * Устанаваливает маркер на карте
 *
 * @param   x Координата X маркера
 * @param   y Координата Y маркера
 * @author  Марунин А.В.
 * @since   2.1
 */
void ddmMapView::setMarker( double x, double y )
{
    if ( this->mapReady() )
    {
        this->evaluateJavaScript( QString( "window.ddmMap.setMarker( %1, %2 );" ).arg( x ).arg( y ) );
    }
}

/**
 * Задает графство, которое должно отобразиться на карте
 *
 * @param   county_id Идентификатор графства
 * @author  Марунин А.В.
 * @since   2.7
 */
void ddmMapView::fitCounty( int county_id )
{
    if ( this->mapReady() )
    {
        this->evaluateJavaScript( QString( "window.ddmMap.fitCounty( %1 );" ).arg( county_id ) );
    }
}

/**
 *
 *
 */
void ddmMapView::slotReplyFinished( QNetworkReply* reply )
{
    Q_UNUSED( reply );
}


/**
 * Добавляет объект с заданным именем к стандартному объекту window в JavaScript
 *
 * @param   name Имя переменной
 * @param   object Указатель на передаваемый объект
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmMapView::addToJavaScriptWindowObject( const QString& name, QObject* object )
{
    if ( this->page() )
    {
        this->page()->mainFrame()->addToJavaScriptWindowObject( name, object );
    }
}

/**
 * Выполняет произвольный JavaScript-код в браузере
 *
 * @param   scriptSource Строка JavaScript-кода
 * @return  Объект типа QVariant - результат выполнения кода
 * @author  Марунин А.В.
 * @since   2.0
 */
QVariant ddmMapView::evaluateJavaScript( const QString& scriptSource )
{
    QVariant result;
    if ( this->page() )
    {
        result = this->page()->mainFrame()->evaluateJavaScript( scriptSource );
    }

    return result;
}

/**
 * Обработчик события отображения окна браузера
 *
 * Переопределяет стандартное поведение
 *
 * @author  Марунин А.В.
 * @since   2.3
 */
void ddmMapView::show()
{
    this->setParent( ddmApp->centralWidget() ); // родителя для карты нужно устанавливать именно при активации фильтра

    QWebView::show();
    this->m_mapLayout->addWidget( this );
}

/**
 * Обработчик события скрытия окна браузера
 *
 * Переопределяет стандартное поведение
 *
 * @author  Марунин А.В.
 * @since   2.3
 */
void ddmMapView::hide()
{
    QWebView::hide();
    this->m_mapLayout->removeWidget( this );
}

/**
 * Обработчик события изменения размеров окна браузера
 *
 * Служит для передачи события в JavaScript
 *
 * @param   width Новая ширина окна
 * @param   height Новая высота окна
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmMapView::resize( int width, int height )
{
    Q_EMIT resized( width, height );
}

/**
 * Доступ к фильтру, ассоциированному с браузером
 *
 * @return  Укатазатель на объект типа ddmFilter
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmFilter* ddmMapView::filter() const
{
    return this->m_filter;
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmMapView::~ddmMapView()
{
}
