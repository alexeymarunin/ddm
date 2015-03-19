#ifndef DDM_BASE_COUNTY_FILTER_H
#define DDM_BASE_COUNTY_FILTER_H

#include <QStringList>
#include "filters/ddmFilter.h"

class ddmState;
class ddmCounty;

/**
 * Класс ddmBaseCountyFilter описывает базовый фильтр
 * при работе с конкретным графством
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
class ddmBaseCountyFilter : public ddmFilter
{
    Q_OBJECT

public:

    ddmBaseCountyFilter( QObject* parent = 0 );

    virtual ~ddmBaseCountyFilter();

protected Q_SLOTS:

    virtual void slotWidgetChangedState();
    virtual void slotWidgetChangedCounty();
    virtual void slotMapLoaded();

protected:

    virtual void setup();
    virtual void updateData( bool fromWidget = true );

    void fitSelection();

    virtual void loadSettings();
    virtual void saveSettings();

};

#endif // DDM_BASE_COUNTY_FILTER_H
