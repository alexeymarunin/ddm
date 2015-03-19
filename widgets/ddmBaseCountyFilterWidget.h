#ifndef DDM_BASE_COUNTY_FILTER_WIDGET_H
#define DDM_BASE_COUNTY_FILTER_WIDGET_H

#include <QComboBox>
#include "widgets/ddmFilterWidget.h"

class ddmBaseCountyFilter;

namespace Ui {
    class ddmBaseCountyFilterWidget;
}

/**
 * Класс ddmBaseCountyFilterWidget описывает базовый виджет для задания параметров фильтра ddmCountyFilter
 *
 * Содержит следующие элементы управления:
 *  - список QComboBox для выбора штата
 *  - список QComboBox для выбора графства
 * При изменении штата или графства происходит автоматическое применение фильтра
 * Предполагается, что в наследуемых классах будет встраиваться панель панель с настройками - сделать это
 * можно с помощью метода insertWidget()
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
class ddmBaseCountyFilterWidget : public ddmFilterWidget
{
    Q_OBJECT

public:

    ddmBaseCountyFilterWidget( ddmBaseCountyFilter* filter, QWidget* parent = NULL );

    QString currentState() const;
    void setCurrentState( const QString& state );

    QString currentCounty() const;
    void setCurrentCounty( const QString& county );

    void setStateNames( const QStringList& names );
    void setCountyNames( const QStringList& names );

    QComboBox* comboState() const;
    QComboBox* comboCounty() const;

    virtual ~ddmBaseCountyFilterWidget();

Q_SIGNALS:

    void changedState();
    void changedCounty();

protected:
    void insertWidget( QWidget* widget );

private:

    Ui::ddmBaseCountyFilterWidget* ui;

};

#endif // DDM_BASE_COUNTY_FILTER_WIDGET_H
