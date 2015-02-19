#ifndef DDM_WIDGET_H
#define DDM_WIDGET_H

#include <QVector>
#include <QWidget>

class QLineEdit;

class ddmFilter;
class ddmState;
class ddmCounty;
class ddmModel;
class ddmMapView;
class ddmInfoWindowWidget;

namespace Ui {
    class ddmWidget;
};

enum ddmFilters
{
    DDM_EMPTY_FILTER,
    DDM_FRICTION_COUNTY_FILTER,
    DDM_MIGRATION_COUNTY_FILTER
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

    void slotInjectModel();

    // изменилось текущие положение курсора на карте (значение координат)
    void changedCoords( const QString& lat ,const QString& lon );

    void slotSetCurrentFilter( int index );

private:

    void  fillFiltersList();

    void installEvents();

    ddmMapView* mapView() const;

private:
    Ui::ddmWidget* ui;

    ddmModel*   m_model;

    QWidget* m_curWidget;

    QVector<ddmFilter*> m_filters;

    ddmInfoWindowWidget* m_infoWindow;

};

#endif // DDM_WIDGET_H
