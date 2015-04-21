#include <QDebug>
#include "ddmVersion.h"
#include "ddmSettings.h"
#include "ddmMainWindow.h"
#include "widgets/ddmCentralWidget.h"
#include "ddmApplication.h"

#include "filters/ddmEmptyFilter.h"
#include "filters/ddmCountyInfoFilter.h"
#include "filters/ddmMinMaxFrictionFilter.h"
#include "filters/ddmPosNegDeltaFilter.h"
#include "filters/ddmFrictionDeltaFilter.h"
#include "filters/ddmFrictionPopulationFilter.h"
#include "filters/ddmDeltaPopulationFilter.h"
#include "filters/ddmNeighborCountyFilter.h"
#include "filters/ddmInEstimateCountyFilter.h"
#include "filters/ddmOutEstimateCountyFilter.h"
#include "filters/ddmMaxInOutNeighborFilter.h"

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
    QCoreApplication::setOrganizationName( "Fake Org" );
    QCoreApplication::setOrganizationDomain( "fake.com" );
    QCoreApplication::setApplicationName( "Distance Decay Map" );
    QCoreApplication::setApplicationVersion( this->version() );

    this->loadSettings();

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
    this->mainWindow()->show();


    // Создаем и настраиваем центральный виджет приложения
    ddmCentralWidget* centralWidget = new ddmCentralWidget( this->mainWindow() );
    this->mainWindow()->setCentralWidget( centralWidget );

    // Фильтры

    centralWidget->appendFilter( "<выберите фильтр>",                   new ddmEmptyFilter( this ) );
    centralWidget->appendFilter( "Информация о графстве",               new ddmCountyInfoFilter( this ) );
    centralWidget->appendFilter( "Диапазон трений",                     new ddmMinMaxFrictionFilter( this ) );
    centralWidget->appendFilter( "Центры миграции",                     new ddmPosNegDeltaFilter( this ) );
    centralWidget->appendFilter( "Диапазон трений по центрам миграции", new ddmFrictionDeltaFilter( this ) );
    centralWidget->appendFilter( "Диапазон трений и населения",         new ddmFrictionPopulationFilter( this ) );
    centralWidget->appendFilter( "Процент дельты от населения",         new ddmDeltaPopulationFilter( this ) );
    centralWidget->appendFilter( "Соседи 1-го порядка",                 new ddmNeighborCountyFilter( this ) );
    centralWidget->appendFilter( "Миграция в графство",                 new ddmInEstimateCountyFilter( this ) );
    centralWidget->appendFilter( "Миграция из графства",                new ddmOutEstimateCountyFilter( this ) );
    centralWidget->appendFilter( "Графства-книппели",                   new ddmMaxInOutNeighborFilter( this ) );


    //centralWidget->setCurrentFilter( 0 );
    ddmSettings* settings = ddmSettings::instance();
    QString lastFilter = settings->value( "panel/filter", "" ).toString();
    centralWidget->setCurrentFilter( lastFilter );
}

/**
 * Загружает настройки приложения
 *
 * @author  Марунин А.В.
 * @since   2.6
 */
void ddmApplication::loadSettings()
{
    ddmSettings* settings = ddmSettings::instance();
    Q_UNUSED( settings );
}

/**
 * Сохраняет настройки приложения
 *
 * @author  Марунин А.В.
 * @since   2.6
 */
void ddmApplication::saveSettings()
{
    ddmSettings* settings = ddmSettings::instance();
    settings->setValue( "version", this->version() );
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
 *
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

    // Сохраняем настройки приложения
    this->saveSettings();

    delete this->m_mainWindow;
}

