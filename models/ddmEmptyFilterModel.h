#ifndef DDM_EMPTY_FILTER_MODEL_H
#define DDM_EMPTY_FILTER_MODEL_H

#include "models/ddmFilterModel.h"

/**
 * Класс ddmEmptyFilterModel определяет модель фильтра ddmEmptyFilter
 *
 * Переопределяет метод load(), запрещая загрузку данных из БД, что значительно
 * ускоряет работу фильтра
 *
 * @author  Марунин А.В.
 * @since   2.2
 */
class ddmEmptyFilterModel : public ddmFilterModel
{
    Q_OBJECT

public:

    ddmEmptyFilterModel( QObject* parent = 0 );

    virtual void load();

    virtual ~ddmEmptyFilterModel();

};

#endif // DDM_EMPTY_FILTER_MODEL_H
