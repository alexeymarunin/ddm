#ifndef DDM_EMPTY_FILTER_H
#define DDM_EMPTY_FILTER_H

#include "filters/ddmFilter.h"

/**
 * Класс ddmEmptyFilter описывает "пустой" фильтр
 * Данный фильтр используется в качестве "заглушки" для отображения пустой карты Google Map (без графств)
 *
 * @author  Марунин А.В.
 * @since   2.2
 */
class ddmEmptyFilter : public ddmFilter
{
    Q_OBJECT

public:

    ddmEmptyFilter( QObject* parent = NULL );

    virtual ~ddmEmptyFilter();

protected Q_SLOTS:

    virtual void slotMapLoaded();

protected:

    virtual void setup();

};

#endif // DDM_EMPTY_FILTER_H
