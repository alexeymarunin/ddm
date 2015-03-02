#ifndef DDM_COUNTY_FILTER_WIDGET_H
#define DDM_COUNTY_FILTER_WIDGET_H

#include <QComboBox>
#include "widgets/ddmFilterWidget.h"

class ddmCountyFilter;

namespace Ui {
    class ddmCountyFilterWidget;
}

/**
 * Класс ddmCountyFilterWidget описывает виджет для задания параметров фильтра ddmCountyFilter
 *
 * Содержит следующие элементы управления:
 *  - список QComboBox для выбора штата
 *  - список QComboBox для выбора графства
 * При изменении штата или графства происходит автоматическое применение фильтра
 *
 * @author  Марунин А.В.
 * @since   2.1
 */
class ddmCountyFilterWidget : public ddmFilterWidget
{
    Q_OBJECT

public:

    ddmCountyFilterWidget( ddmCountyFilter* countyFilter, QWidget* parent = NULL );

    QString currentState() const;
    void setCurrentState( const QString& state );

    QString currentCounty() const;
    void setCurrentCounty( const QString& county );

    void setStateNames( const QStringList& names );
    void setCountyNames( const QStringList& names );

    QComboBox* comboState() const;
    QComboBox* comboCounty() const;

    virtual ~ddmCountyFilterWidget();

Q_SIGNALS:

    void changedState();
    void changedCounty();

private:

    Ui::ddmCountyFilterWidget* ui;

};

#endif // DDM_COUNTY_FILTER_WIDGET_H
