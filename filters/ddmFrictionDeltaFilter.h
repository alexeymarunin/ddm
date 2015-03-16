#ifndef DDMFRICTIONDELTAFILTER
#define DDMFRICTIONDELTAFILTER

#include "filters/ddmFilter.h"


class ddmFrictionDeltaFilter: public ddmFilter
{
    Q_OBJECT
public:

    ddmFrictionDeltaFilter( QObject* parent = NULL );

    virtual ~ddmFrictionDeltaFilter();

protected:

    virtual void setup();
    virtual void updateData( bool fromWidget = true );
    virtual void updateSelection();

    virtual void loadSettings();
    virtual void saveSettings();
};


#endif // DDMFRICTIONDELTAFILTER
