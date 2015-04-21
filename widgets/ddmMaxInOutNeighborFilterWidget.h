#ifndef DDM_MAX_IN_OUT_NEIGHBOR_FILTER_WIDGET_H
#define DDM_MAX_IN_OUT_NEIGHBOR_FILTER_WIDGET_H

#include <QComboBox>
#include <QCheckBox>
#include <QListWidget>
#include "base/ddmDefines.h"
#include "widgets/ddmFilterWidget.h"

class ddmMaxInOutNeighborFilter;

namespace Ui {
    class ddmMaxInOutNeighborFilterWidget;
}

/**
 * Класс ddmMaxInOutNeighborFilterWidget описывает виджет для фильтра ddmMaxInOutNeighborFilter
 *
 * @author  Марунин А.В.
 * @since   2.10
 */
class ddmMaxInOutNeighborFilterWidget : public ddmFilterWidget
{
    Q_OBJECT

public:

    ddmMaxInOutNeighborFilterWidget( ddmMaxInOutNeighborFilter* filter, QWidget* parent = NULL );

    void setNeighborType( ddmNeighborType type );
    ddmNeighborType neighborType() const;
    QComboBox* neighborTypeCombo() const;

    void checkSameState( bool checked = true );
    bool sameStateChecked() const;
    QCheckBox* sameStateCheckbox() const;

    void setCounties( const QStringList& countyNames );
    void setCurrentCounty( const QString& countyName );
    QString currentCounty() const;
    QListWidget* countryList() const;

    virtual ~ddmMaxInOutNeighborFilterWidget();

Q_SIGNALS:
    void changeCounty();

private:

    Ui::ddmMaxInOutNeighborFilterWidget* ui;
};

#endif // DDM_MAX_IN_OUT_NEIGHBOR_FILTER_WIDGET_H
