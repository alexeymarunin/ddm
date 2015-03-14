#ifndef DDM_DELTA_POPULATION_FILTER_MODEL_H
#define DDM_DELTA_POPULATION_FILTER_MODEL_H

#include "models/ddmFilterModel.h"


class ddmDeltaPopulationFilterModel: public ddmFilterModel
{
    Q_OBJECT

public:

    ddmDeltaPopulationFilterModel( QObject* parent = 0 );

    virtual void reloadData();

    double minBound() const;
    void setMinBound( double bound );

    double maxBound() const;
    void setMaxBound( double bound );

    void setBounds( double minBound, double maxBound );

    double minPopulation() const;
    double maxPopulation() const;

    virtual ~ddmDeltaPopulationFilterModel();

protected:

    double  m_minBound;
    double  m_maxBound;

    double  m_minPopulatiom;    // в процентах
    double  m_maxPopulatiom;   // в процентах
};

#endif // DDM_DELTA_POPULATION_FILTER_MODEL_H

