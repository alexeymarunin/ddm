#ifndef DDM_IN_ESTIM_AND_POPUL_FILTER_MODEL_H
#define DDM_IN_ESTIM_AND_POPUL_FILTER_MODEL_H

#include "models/ddmBaseCountyFilterModel.h"


class ddmInEstimAndPopulFilterModel : public ddmBaseCountyFilterModel
{
    Q_OBJECT

public:

    ddmInEstimAndPopulFilterModel( QObject* parent = 0 );

    virtual void reloadData();

    double minPopBound() const;
    void setMinPopBound( double bound );

    double maxPopBound() const;
    void setMaxPopBound( double bound );

    void setPopBounds( double minBound, double maxBound );

    double minPopulation() const;
    double maxPopulation() const;

    QSet<int> counties() const;

    qlonglong getCurentCountyPopulation();

    virtual ~ddmInEstimAndPopulFilterModel();

private:
    QSet<int> m_counties;

    double m_minPopBound;
    double m_maxPopBound;

    double  m_minPopulation;
    double  m_maxPopulation;

    void updateCounties();

};

#endif // DDM_IN_ESTIM_AND_POPUL_FILTER_MODEL_H

