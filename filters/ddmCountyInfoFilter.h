#ifndef DDM_COUNTY_FILTER_H
#define DDM_COUNTY_FILTER_H

#include <QStringList>
#include "filters/ddmBaseCountyFilter.h"

class ddmState;
class ddmCounty;

/**
 * Класс ddmCountyInfoFilter описывает фильтр для отображения информации выбранного графства
 *
 * @author  Марунин А.В.
 * @since   2.1
 */
class ddmCountyInfoFilter : public ddmBaseCountyFilter
{
    Q_OBJECT

public:

    ddmCountyInfoFilter( QObject* parent = 0 );

    virtual ~ddmCountyInfoFilter();

protected Q_SLOTS:

    virtual void slotWidgetChangedCounty();

protected:

    virtual void setup();
    virtual void updateSelection();

    void updateInfo();


};

#endif // DDM_COUNTY_FILTER_H
