#ifndef DDM_FILTER_WIDGET_H
#define DDM_FILTER_WIDGET_H

#include <QWidget>
#include "ddmMapView.h"

class ddmFilter;

/**
 * Класс ddmFilterWidget является базовым классом для всех остальных
 * виджетов, которые используются для задания параметров фильтров
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
class ddmFilterWidget : public QWidget
{
    Q_OBJECT

public:

    ddmFilterWidget( ddmFilter* filter, QWidget* parent = NULL );

    ddmFilter* filter() const;
    ddmMapView* mapView() const;

    virtual ~ddmFilterWidget();

Q_SIGNALS:

    void changed();

public Q_SLOTS:

    void slotApplyFilter();

protected:

    ddmFilter*  m_filter;
    ddmMapView* m_mapView;  // TODO: на мой взгляд, mapView нужно перенести отсюда в класс фильтра

};

#endif // DDM_FILTER_WIDGET_H
