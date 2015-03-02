#include "models/ddmFilterModel.h"


/**
 * @brief ddmFilterModel::ddmFilterModel
 * @param parent
 */
ddmFilterModel::ddmFilterModel( QObject* parent ) : ddmStateModel( parent )
{
    this->setObjectName( "model" ); // делаем модель доступной в JavaScript
}

/**
 * @brief ddmFilterModel::~ddmFilterModel
 */
ddmFilterModel::~ddmFilterModel()
{
}

