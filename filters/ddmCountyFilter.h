#ifndef DDM_COUNTY_FILTER_H
#define DDM_COUNTY_FILTER_H

#include <QStringList>
#include "filters/ddmFilter.h"

class ddmState;
class ddmCounty;

/**
 * Класс ddmCountyFilter описывает фильтр для отображения одного графства
 *
 * @author  Марунин А.В.
 * @since   2.1
 */
class ddmCountyFilter : public ddmFilter
{
    Q_OBJECT

public:

    ddmCountyFilter( QObject* parent = 0 );

    virtual ~ddmCountyFilter();

protected Q_SLOTS:

    void slotWidgetChangedState();
    void slotWidgetChangedCounty();
    virtual void slotMapLoaded();

protected:

    virtual void create();
    virtual void updateData( bool fromWidget = true );
    virtual void updateSelection();

};

#endif // DDM_COUNTY_FILTER_H
