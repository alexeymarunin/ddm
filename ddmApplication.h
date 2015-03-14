#ifndef DDM_APPLICATION_H
#define DDM_APPLICATION_H

#include <QApplication>

class ddmMainWindow;
class ddmCentralWidget;

/**
 * Класс ddmApplication описывает приложение
 * Предоставляет доступ к центральному виджету и (в перспективе) некоторые другие возможности
 *
 * @author  Марунин А.В.
 * @since   2.1
 */
class ddmApplication : public QApplication
{
    Q_OBJECT

public:

    ddmApplication( int& argc, char** argv );

    QString version() const;
    ddmMainWindow* mainWindow() const;
    ddmCentralWidget* centralWidget() const;

    virtual ~ddmApplication();

Q_SIGNALS:

public Q_SLOTS:

private:
    ddmMainWindow*  m_mainWindow;

    void loadSettings();
    void saveSettings();

};

/**
 * Специальная переменная ddmApp может быть использована для доступа к экземпляру приложения
 *
 * @author  Марунин А.В.
 * @since   2.1
 */
#define ddmApp ( static_cast<ddmApplication*>( qApp ) )

#endif // DDM_APPLICATION_H
