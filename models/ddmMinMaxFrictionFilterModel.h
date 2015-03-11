#ifndef DDM_MIN_MAX_FRICTION_FILTER_MODEL_H
#define DDM_MIN_MAX_FRICTION_FILTER_MODEL_H

#include "models/ddmFilterModel.h"

/**
 * Класс ddmMinMaxFrictionFilterModel описывает модель данных для фильтра ddmMinMaxFrictionFilter
 *
 * Основными элементами модели являются:
 *  - minFriction минимально допустимое значение трения
 *  - minFriction максимально допустимое значение трения
 *  - minBound "нижняя граница" значения трения
 *  - maxBound "верхняя граница" значения трения
 *
 * Минимально и максимально допустимые значения определяются автоматически во время загрузки модели,
 * и в дальнейшем изменить их невозможно (доступ только на чтение)
 *
 * При изменении minBound или maxBound испускается сигнал changed()
 *
 * @author  Марунин А.В.
 * @since   2.3
 */
class ddmMinMaxFrictionFilterModel : public ddmFilterModel
{
    Q_OBJECT

public:

    ddmMinMaxFrictionFilterModel( QObject* parent = 0 );

    virtual void reloadData();

    double minBound() const;
    void setMinBound( double bound );

    double maxBound() const;
    void setMaxBound( double bound );

    void setBounds( double minBound, double maxBound );

    double minFriction() const;
    double maxFriction() const;

    virtual ~ddmMinMaxFrictionFilterModel();

protected:

    double  m_minBound;
    double  m_maxBound;

    double  m_minFriction;
    double  m_maxFriction;

    void updateMinMaxFrictions();

};

#endif // DDM_MIN_MAX_FRICTION_FILTER_MODEL_H
