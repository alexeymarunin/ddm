#ifndef DDM_MODEL_H
#define DDM_MODEL_H

#include "base/ddmObject.h"
#include "base/ddmDatabase.h"

class ddmDatabase;

/**
 * Класс ddmModel описывает базовую модель
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
class ddmModel : public ddmObject
{
    Q_OBJECT

public:

    ddmModel( QObject* parent = 0 );

    ddmDatabase& database() const;

    virtual ~ddmModel();

Q_SIGNALS:

    void changed();
};

#endif // DDM_MODEL_H
