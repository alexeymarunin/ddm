#ifndef DDM_WIDGET_H
#define DDM_WIDGET_H

#include <QWidget>

class QLineEdit;

class ddmState;
class ddmCounty;
class ddmModel;
class ddmMapView;

namespace Ui {
    class ddmWidget;
};


class ddmWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ddmWidget( ddmModel* model = 0, QWidget* parent = 0 );

    void setModel( ddmModel* model );
    ddmModel* model() const;

    virtual ~ddmWidget();

    // перезагрузить страницу
    void reload();

    // увеличить масштаб
    void increaseZoom();

    // уменьшить масштаб
    void decreaseZoom();

protected:
    void resizeEvent( QResizeEvent *event );

signals:
    // изменить значение координат в StatusBar'е
    void changedStatusBarCoords( const QString& lat, const QString& lng );

private slots:

    // установить текуший штат для модели
    void slotSetCurrentState( const QString& stateName );

    // установить текушее графство для модели
    void slotSetCurrentCounty( const QString& countyName );

    void slotInjectModel();

    // обновить штат
    void changedState( ddmState* state );

    // обновить графство на карте
    void changedCounty( ddmCounty* county );

    // изменилось текущие положение курсора на карте (значение координат)
    void changedCoords( const QString& lat ,const QString& lon );

private:
    Ui::ddmWidget* ui;

    ddmModel*   m_model;
    QLineEdit*  m_leState;
    QLineEdit*  m_leCounty;

    ddmMapView* mapView() const;

    void installEvents();
    void updateCountiesList();

};

#endif // DDM_WIDGET_H
