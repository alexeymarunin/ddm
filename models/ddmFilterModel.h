#ifndef DDM_FILTER_MODEL_H
#define DDM_FILTER_MODEL_H

#include "models/ddmStateModel.h"

/**
 * Класс ddmFilterModel является базовым классом модели любого фильтра
 * Предоставляет доступ к модели фильтра в JavaScript (через поле model)
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
class ddmFilterModel : public ddmStateModel
{
    Q_OBJECT

public:

    ddmFilterModel( QObject* parent = 0 );

    virtual ~ddmFilterModel();

};

#endif // DDM_FILTER_MODEL_H
