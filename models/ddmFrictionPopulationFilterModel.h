#ifndef DDM_FRICTION_POPULATION_FILTER_MODEL_H
#define DDM_FRICTION_POPULATION_FILTER_MODEL_H


#include "models/ddmFilterModel.h"


class ddmFrictionPopulationFilterModel : public ddmFilterModel
{
    Q_OBJECT

public:

    ddmFrictionPopulationFilterModel( QObject* parent = 0 );

    virtual ~ddmFrictionPopulationFilterModel();

    virtual void reloadData();

    double minFrBound() const;
    void setMinFrBound( double bound );

    double maxFrBound() const;
    void setMaxFrBound( double bound );

    void setFrBounds( double minBound, double maxBound );

    double minFriction() const;
    double maxFriction() const;


    double minPopBound() const;
    void setMinPopBound( double bound );

    double maxPopBound() const;
    void setMaxPopBound( double bound );

    void setPopBounds( double minBound, double maxBound );

    double minPopulation() const;
    double maxPopulation() const;


protected:

    double  m_minFrBound;
    double  m_maxFrBound;

    double  m_minFriction;
    double  m_maxFriction;

    double  m_minPopBound;
    double  m_maxPopBound;

    double  m_minPopulation;
    double  m_maxPopulation;

    void updateMinMaxFrictions();

    void updateMinMaxPopulation();

};


#endif // DDM_FRICTION_POPULATION_FILTER_MODEL_H

