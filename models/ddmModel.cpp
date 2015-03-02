#include "models/ddmModel.h"

/**
 * @brief ddmModel::ddmModel
 * @param parent
 */
ddmModel::ddmModel( QObject* parent ) : ddmObject( parent )
{
}

/**
 * @brief ddmModel::database
 * @return
 */
ddmDatabase& ddmModel::database() const
{
    return ddmDatabase::instance();
}

/**
 * @brief ddmModel::~ddmModel
 */
ddmModel::~ddmModel()
{
}

