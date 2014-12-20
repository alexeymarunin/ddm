#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ddmModel.h"

class ddmWidget;

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
    void CreateMenuEvents();

private slots:
    void slotQuit();

    void slotOpen();

private:
    Ui::MainWindow *ui;

    ddmWidget* m_viewMapWidget;

    ddmModel m_model;


};

#endif // MAINWINDOW_H
