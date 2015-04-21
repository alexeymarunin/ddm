#ifndef DDM_MAX_IN_OUT_NEIGHBOR_FILTER_H
#define DDM_MAX_IN_OUT_NEIGHBOR_FILTER_H

#include "filters/ddmFilter.h"

/**
 * Класс ddmMaxInOutNeighborFilter описывает фильтр, позволяющий отображать графства,
 *
 *
 * @author  Марунин А.В.
 * @since   2.10
 */
class ddmMaxInOutNeighborFilter : public ddmFilter
{
    Q_OBJECT

public:

    ddmMaxInOutNeighborFilter( QObject* parent = NULL );

    virtual ~ddmMaxInOutNeighborFilter();

protected Q_SLOTS:

    virtual void slotWidgetChanged();

protected:

    virtual void setup();
    virtual void updateData( bool fromWidget = true );
    virtual void updateSelection();

    virtual void loadSettings();
    virtual void saveSettings();
};

#endif // DDM_MAX_IN_OUT_NEIGHBOR_FILTER_H
