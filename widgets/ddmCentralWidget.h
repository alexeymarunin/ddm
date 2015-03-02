#ifndef DDM_CENTRAL_WIDGET_H
#define DDM_CENTRAL_WIDGET_H

#include <QVBoxLayout>
#include <QComboBox>
#include <QWidget>

class ddmFilter;
class ddmMapView;

namespace Ui {
    class ddmCentralWidget;
};


/**
 * Класс ddmCentralWidget описывает класс центрального (основного) виджета главного окна приложения
 *
 * В данном классе выполняется основная работа со списком фильтров (создание, переключение)
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
class ddmCentralWidget : public QWidget
{
    Q_OBJECT

public:

    ddmCentralWidget( QWidget* parent = 0 );

    QVBoxLayout* widgetLayout() const;
    QVBoxLayout* mapLayout() const;

    ddmFilter* currentFilter() const;
    void setCurrentFilter( int index );

    QComboBox* comboFilter() const;

    // перезагрузить страницу
    void reload();

    // увеличить масштаб
    void increaseZoom();

    // уменьшить масштаб
    void decreaseZoom();

    virtual ~ddmCentralWidget();

public Q_SLOTS:

    void slotChangedFilter( int index );

protected:

    ddmFilter*  m_currentFilter;

    void resizeEvent( QResizeEvent *event );
    void fillFilterCombo();
    void installEvents();

private:

    Ui::ddmCentralWidget* ui;

};

#endif // DDM_CENTRAL_WIDGET_H
