#ifndef DDM_MIN_MAX_FRICTION_FILTER_H
#define DDM_MIN_MAX_FRICTION_FILTER_H

#include "filters/ddmFilter.h"

/**
 * Класс ddmMinMaxFrictionFilter описывает фильтр, позволяющий отображать графства,
 * значение трения которого (поле f_mid) попадает в заданный диапазон
 *
 * @author  Марунин А.В.
 * @since   2.3
 */
class ddmMinMaxFrictionFilter : public ddmFilter
{
    Q_OBJECT

public:

    ddmMinMaxFrictionFilter( QObject* parent = NULL );

    virtual ~ddmMinMaxFrictionFilter();

protected:

    virtual void setup();
    virtual void updateData( bool fromWidget = true );
    virtual void updateSelection();

    virtual void loadSettings();
    virtual void saveSettings();
};

#endif // DDM_MIN_MAX_FRICTION_FILTER_H
