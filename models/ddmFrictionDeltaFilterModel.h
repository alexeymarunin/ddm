#ifndef DDM_FRICTION_DELTA_FILTER_MODEL_H
#define DDM_FRICTION_DELTA_FILTER_MODEL_H

#include "models/ddmFilterModel.h"


class ddmFrictionDeltaFilterModel: public ddmFilterModel
{
    Q_OBJECT

public:

    ddmFrictionDeltaFilterModel( QObject* parent = 0 );

    virtual void reloadData();

    double minBound() const;
    void setMinBound( double bound );

    double maxBound() const;
    void setMaxBound( double bound );

    void setBounds( double minBound, double maxBound );

    double minFriction() const;
    double maxFriction() const;

    void setDeltaMode( int mode );
    int  deltaMode();

    virtual ~ddmFrictionDeltaFilterModel();

protected:

    double  m_minBound;
    double  m_maxBound;

    double  m_minFriction;
    double  m_maxFriction;

    int m_deltaMode;

    void updateMinMaxFrictions();

};

#endif //DDM_FRICTION_DELTA_FILTER_MODEL_H
