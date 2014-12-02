#ifndef DDMMAPSWIDGET_H
#define DDMMAPSWIDGET_H

#include <QWidget>

class QLineEdit;
class ddmObjectModel;

namespace Ui {
class ddmMapsWidget;
}

class ddmMapsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ddmMapsWidget(QWidget *parent = 0 );
    virtual ~ddmMapsWidget();

private:
    // инициализировать обработчики событий
    void installEvents();

    // обновить список графств, в зависимости от штата
    void updateCountiesList( const QString& state );

private slots:
    // установить текуший штат для модели
    void slotSetCurentState( const QString&  text );

    // установить текушее графство для модели
    void slotSetCurentCounty( const QString& text );

    // установить текуший штат для модели
    void slotSetCurentState();

    // установить текушее графство для модели
    void slotSetCurentCounty();

    // обновить штат на карте
    void updateStateOnMap(  const QString& state );

    // обновить графство на карте
    void updateCountyOnMap( const QString& county );

private:
    Ui::ddmMapsWidget *ui;

    QLineEdit* m_leState;
    QLineEdit* m_leCounty;

    ddmObjectModel* m_model;
};

#endif // DDMMAPSWIDGET_H
