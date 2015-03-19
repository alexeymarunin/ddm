#ifndef DDMPOSNEGDELTAFILTER
#define DDMPOSNEGDELTAFILTER

#include "filters/ddmFilter.h"

/**
 * Класс ddmPosNegDeltaFilter описывает фильтр, позволяющий отображать графства,
 * с положительной или отрицательной дельтой (приехавшие в графство - уехавшие из него)
 *
 * @author  Цалко Т.В.
 * @since   2.3
 */
class ddmPosNegDeltaFilter: public ddmFilter
{
    Q_OBJECT

public:

    ddmPosNegDeltaFilter( QObject* parent = NULL );

    virtual ~ddmPosNegDeltaFilter();

protected:

    virtual void setup();
    virtual void updateData( bool fromWidget = true );
    virtual void updateSelection();

    virtual void loadSettings();
    virtual void saveSettings();

};


#endif // DDMPOSNEGDELTAFILTER

