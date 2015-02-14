#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ddmModel.h"

class ddmWidget;
class ddmTableView;
class QLabel;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget* parent = 0 );

    ddmModel* model() const;

    ~MainWindow();

private:
    void InstallEvents();

private slots:
    void slotQuit();

    void slotOpen();

    void slotReload();

    void slotIncreaseZoom();

    void slotDecreaseZoom();

    void changedStatusBarCoords( const QString& lat, const QString& lng );

    void createTableView();

private:
    Ui::MainWindow *ui;

    ddmWidget* m_viewMapWidget;

    ddmModel m_model;

    QLabel* m_statusBarLat;
    QLabel* m_statusBarLng;

    QVector<ddmTableView*> m_viewsVector;


};

#endif // MAINWINDOW_H
