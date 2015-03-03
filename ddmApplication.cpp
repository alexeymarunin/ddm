#include <QDebug>
#include "ddmVersion.h"
#include "ddmMainWindow.h"
#include "widgets/ddmCentralWidget.h"
#include "ddmApplication.h"

#include "filters/ddmEmptyFilter.h"
#include "filters/ddmCountyFilter.h"
#include "filters/ddmMinMaxFrictionFilter.h"

/**
 * @Конструктор класса
 *
 * @param   argc Количество аргументов, переданных через командную строку
 * @param   argv Массив аргументов командной строки
 * @author  Марунин А.В.
 * @since   2.1
 */
ddmApplication::ddmApplication( int& argc, char** argv ) : QApplication( argc, argv )
{
    // Инициализируем БД
    ddmDatabase& db = ddmDatabase::instance();
    Q_UNUSED( db );


    // Создаем главное окно приложения
    qDebug() << "App version:" << this->version();
#ifdef QT_DEBUG
    qDebug() << "Mode: Debug";
#else
    qDebug() << "Mode: Release";
#endif
    this->m_mainWindow = new ddmMainWindow;

    QString title = QString( "Distance Decay Map, v.%1" ).arg( this->version() );
#ifdef QT_DEBUG
    title += QString( " [DEBUG MODE]" );
#endif
    this->m_mainWindow->setWindowTitle( title );
    this->m_mainWindow->show();


    // Создаем и настраиваем центральный виджет приложения
    ddmCentralWidget* centralWidget = new ddmCentralWidget( this->mainWindow() );
    this->mainWindow()->setCentralWidget( centralWidget );

    // Фильтры
    centralWidget->appendFilter( "<выберите фильтр>",       new ddmEmptyFilter( this ) );
    centralWidget->appendFilter( "Информация о графстве",   new ddmCountyFilter( this ) );
    centralWidget->appendFilter( "Диапазон трений",         new ddmMinMaxFrictionFilter( this ) );

    //centralWidget->setCurrentFilter( 0 );
}

/**
 * Доступ к главному окну приложения
 *
 * @return  Указатель на объект типа ddmMainWindow
 * @author  Марунин А.В.
 * @since   2.1
 */
ddmMainWindow* ddmApplication::mainWindow() const
{
    return this->m_mainWindow;
}

/**
 * Доступ к центральному виджету главного окна
 *
 * @return  Указатель на объект типа ddmCentralWidget
 * @author  Марунин А.В.
 * @since   2.1
 */
ddmCentralWidget* ddmApplication::centralWidget() const
{
    return qobject_cast<ddmCentralWidget*>( this->mainWindow()->centralWidget() );
}

/**
 * Возвращает текущую версию приложения
 * @return  Строка - версия приложения
 * @author  Марунин А.В.
 * @since   2.1
 */
QString ddmApplication::version() const
{
    return QString( "%1.%2.%3" ).arg( DDM_MAJOR_VERSION ).arg( DDM_MINOR_VERSION ).arg( DDM_BUILD_NUMBER );
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.3
 */
ddmApplication::~ddmApplication()
{
    // Закрываем БД
    ddmDatabase::instance().close();
}
