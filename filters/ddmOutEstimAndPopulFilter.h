#ifndef DDM_OUT_ESTIM_AND_POPUL_FILTER_H
#define DDM_OUT_ESTIM_AND_POPUL_FILTER_H

#include "filters/ddmBaseCountyFilter.h"


class ddmOutEstimAndPopulFilter : public ddmBaseCountyFilter
{
    Q_OBJECT

public:

    ddmOutEstimAndPopulFilter( QObject* parent = 0 );

    virtual ~ddmOutEstimAndPopulFilter();

    virtual void saveSettings();
    virtual void loadSettings();

protected:

    virtual void setup();
    virtual void updateData( bool fromWidget = true );
    virtual void updateSelection();
};


#endif //DDM_OUT_ESTIM_AND_POPUL_FILTER_H

