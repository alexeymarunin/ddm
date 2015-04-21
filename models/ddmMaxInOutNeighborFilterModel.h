#ifndef DDM_MAX_IN_OUT_NEIGHBOR_FILTER_MODEL_H
#define DDM_MAX_IN_OUT_NEIGHBOR_FILTER_MODEL_H

#include <QList>
#include "base/ddmDefines.h"
#include "models/ddmFilterModel.h"

/**
 * Класс ddmMaxInOutNeighborFilterModel описывает модель данных для фильтра ddmMaxInOutNeighborFilter
 *
 * Модель находит все графства, у которых соседи 1-го порядка являются макс. по числу приехавших и уехавших
 * Issue #38
 *
 * @author  Марунин А.В.
 * @since   2.10
 */
class ddmMaxInOutNeighborFilterModel : public ddmFilterModel
{
    Q_OBJECT

public:


    ddmMaxInOutNeighborFilterModel( QObject* parent = 0 );

    ddmNeighborType neighborType() const;
    void setNeighborType( ddmNeighborType type );

    bool sameState() const;
    void setSameState( bool flag );

    void setNeighborOptions( ddmNeighborType neighborType, bool sameState );

    QStringList countyNames() const;
    QVariantList neighbors() const;

    virtual void reloadData();


    virtual ~ddmMaxInOutNeighborFilterModel();

protected:
    ddmNeighborType m_neighborType;
    bool            m_sameState;
    QString         m_tableMaxInCountries;      //<! имя временной таблицы, в которой указаны макс. приехавшие для всех графств
    QString         m_tableMaxOutCountries;     //<! имя временной таблицы, в которой указаны макс. уехавшие для всех графств
    QString         m_tableMaxInOutCountries;   //<! имя временной таблицы, в которой указаны макс. уехавшие и приехавшие для всех графств
    ddmFilterModel* m_neighborFilter;
    QVariantList    m_neighbors;

    void prepareTempTable();
};

#endif // DDM_MAX_IN_OUT_NEIGHBOR_FILTER_MODEL_H
