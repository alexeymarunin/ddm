#ifndef DDMPOSNEGDELTAFILTERMODEL_H
#define DDMPOSNEGDELTAFILTERMODEL_H

#include "models/ddmFilterModel.h"

/**
 * Класс ddmPosNegDeltaFilterModel описывает модель данных для фильтра ddmPosNegDeltaFilter
 *
 * Основными элементами модели являются:
 *  - deltaMode режим отображения дельта (графства с положительным/отрицательным значением дельта)
 *    дельта ( приехавшие в графство - уехавшие из графства )
 * @author  Цалко Т.В.
 * @since   2.3
 */


class ddmPosNegDeltaFilterModel: public ddmFilterModel
{
    Q_OBJECT

public:
    ddmPosNegDeltaFilterModel( QObject* parent = 0 );

    virtual void reloadData();

    void setDeltaMode( int mode );
    int  deltaMode();

    virtual ~ddmPosNegDeltaFilterModel();

private:
    int m_deltaMode;
};


#endif // DDMPOSNEGDELTAFILTERMODEL_H

