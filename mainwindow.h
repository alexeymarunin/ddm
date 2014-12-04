#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ddmMapsWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void CreateMenuEvents();

private slots:
    void slotQuit();

    void slotOpen();

private:
    Ui::MainWindow *ui;

    ddmMapsWidget* m_viewMapWidget;

};

#endif // MAINWINDOW_H
