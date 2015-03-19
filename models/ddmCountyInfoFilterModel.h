#ifndef DDM_COUNTY_INFO_FILTER_MODEL_H
#define DDM_COUNTY_INFO_FILTER_MODEL_H

#include "models/ddmBaseCountyFilterModel.h"

/**
 * Класс ddmCountyInfoFilterModel определяет модель фильтра ddmCountyInfoFilter
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
class ddmCountyInfoFilterModel : public ddmBaseCountyFilterModel
{
    Q_OBJECT

public:

    ddmCountyInfoFilterModel( QObject* parent = 0 );

    virtual ~ddmCountyInfoFilterModel();

protected:

};

#endif // DDM_COUNTY_INFO_FILTER_MODEL_H
