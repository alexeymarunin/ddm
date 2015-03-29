#ifndef DDM_OUT_ESTIMATE_COUNTY_FILTER_MODEL_H
#define DDM_OUT_ESTIMATE_COUNTY_FILTER_MODEL_H

#include "models/ddmBaseCountyFilterModel.h"


class ddmOutEstimateCountyFilterModel : public ddmBaseCountyFilterModel
{
    Q_OBJECT

public:

    ddmOutEstimateCountyFilterModel( QObject* parent = 0 );

    virtual void reloadData();

    double minPopBound() const;
    void setMinPopBound( double bound );

    double maxPopBound() const;
    void setMaxPopBound( double bound );

    void setPopBounds( double minBound, double maxBound );

    double minPopulation() const;
    double maxPopulation() const;

    QSet<int> counties() const;

    virtual ~ddmOutEstimateCountyFilterModel();

private:
    QSet<int> m_counties;

    double m_minPopBound;
    double m_maxPopBound;

    double  m_minPopulation;
    double  m_maxPopulation;

    void updateCounties();

    void updateMinMaxPopulation();
};


#endif // DDM_OUT_ESTIMATE_COUNTY_FILTER_MODEL_H

