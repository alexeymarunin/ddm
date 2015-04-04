#ifndef DDM_IN_ESTIM_AND_POPUL_FILTER_H
#define DDM_IN_ESTIM_AND_POPUL_FILTER_H

#include "filters/ddmBaseCountyFilter.h"


class ddmInEstimAndPopulFilter : public ddmBaseCountyFilter
{
    Q_OBJECT

public:

    ddmInEstimAndPopulFilter( QObject* parent = 0 );

    virtual ~ddmInEstimAndPopulFilter();

    virtual void saveSettings();
    virtual void loadSettings();

protected:

    virtual void setup();
    virtual void updateData( bool fromWidget = true );
    virtual void updateSelection();

};


#endif //DDM_IN_ESTIM_AND_POPUL_FILTER_H

