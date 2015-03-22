#ifndef DDMPOSNEGDELTAFILTERWIDGET
#define DDMPOSNEGDELTAFILTERWIDGET

#include "widgets/ddmFilterWidget.h"

class QComboBox;
class ddmPosNegDeltaFilterWidget;
class ddmPosNegDeltaFilter;

namespace Ui {
    class ddmPosNegDeltaFilterWidget;
}

/**
 * Класс ddmPosNegDeltaFilterWidget описывает виджет для задания параметров фильтра ddmPosNegDeltaFilterWidget
 *
 * Содержит следующие элементы управления:
 *  - список QComboBox для задания режима отображения графств с положительным/отрицательным значением дельта = приехавшие - уехавшие
 * @author  Цалко Т.В.
 * @since   2.3
 */

class ddmPosNegDeltaFilterWidget: public ddmFilterWidget
{
    Q_OBJECT

public:

    ddmPosNegDeltaFilterWidget( ddmPosNegDeltaFilter* filter, QWidget* parent = 0 );

    void setDeltaMode( int mode, bool blockSignal = false );

    int deltaMode() const;

    QComboBox* comboDeltaMode() const;

    ~ddmPosNegDeltaFilterWidget();

private:
    Ui::ddmPosNegDeltaFilterWidget* ui;
};

#endif // DDMPOSNEGDELTAFILTERWIDGET

