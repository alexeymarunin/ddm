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

protected:
    void resizeEvent( QResizeEvent *event );

private slots:
    // установить текуший штат для модели
    void slotSetCurentState( const QString&  text );

    // установить текушее графство для модели
    void slotSetCurentCounty( const QString& text );

    // обновить штат
    void updateState(  const QString& state );

    // обновить гравство на карте
    void updateCounty(  const QString& state );

private:
    Ui::ddmMapsWidget *ui;

    QLineEdit* m_leState;
    QLineEdit* m_leCounty;

    ddmObjectModel* m_model;
};

#endif // DDMMAPSWIDGET_H
