#ifndef DDM_MAINWINDOW_H
#define DDM_MAINWINDOW_H

#include <QMainWindow>

class ddmCentralWidget;
class ddmTableView;
class QLabel;

namespace Ui {
    class ddmMainWindow;
}

/**
 * Класс ddmMainWindow описывает главное окно приложения
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
class ddmMainWindow : public QMainWindow
{
    Q_OBJECT

public:

    ddmMainWindow( QWidget* parent = 0 );

    virtual ~ddmMainWindow();

public Q_SLOTS:

    void slotMapMouseMove( double lat, double lng );

private Q_SLOTS:

    void slotQuit();

    // void slotOpen();

    void slotReload();

    void slotIncreaseZoom();

    void slotDecreaseZoom();

    //void createTableView();

protected:

    void InstallEvents();

private:

    Ui::ddmMainWindow*  ui;
    ddmCentralWidget*   m_centralWidget;

    QLabel* m_statusBarLat;
    QLabel* m_statusBarLng;

};

#endif // DDM_MAINWINDOW_H
