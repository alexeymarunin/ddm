#ifndef DDM_COUNTY_FILTER_MODEL_H
#define DDM_COUNTY_FILTER_MODEL_H

#include <QStringList>

#include "models/ddmFilterModel.h"
#include "base/ddmState.h"
#include "base/ddmCounty.h"

/**
 * Класс ddmCountyFilterModel определяет модель фильтра ddmCountyFilter
 * Предоставляет доступ к данным одного конкретного графства
 *
 * Основными элементами модели являются:
 *  - currentState выбранный штат
 *  - currentCounty выбранное графство
 *  - stateNames список имен доступных штатов
 *  - countyNames список имен графств текущего штата
 * При изменении штата или графства испускается сигнал changed()
 * При выборе графства, которое относится к другому штату, текущий штат меняется автоматически
 * Список countyNames автоматически обновляется при изменени штата
 *
 * @author  Марунин А.В.
 * @since   2.1
 */
class ddmCountyFilterModel : public ddmFilterModel
{
    Q_OBJECT

public:

    ddmCountyFilterModel( QObject* parent = 0 );

    virtual void reloadData();

    ddmState* currentState() const;
    void setCurrentState( int state_id );
    void setCurrentState( const QString& geographicName );

    ddmCounty* currentCounty() const;
    void setCurrentCounty( int id );
    void setCurrentCounty( const QString& geographicName );

    QStringList stateNames() const;
    QStringList countyNames() const;

    virtual ~ddmCountyFilterModel();

protected:

    ddmState*           m_currentState;
    ddmCounty*          m_currentCounty;
    QStringList         m_stateNames;
    QStringList         m_countyNames;
    QMap<int, QString>  m_stateMap;

    ddmState* loadState( int state_id );

    void setCurrentState( ddmState* state );
    void setCurrentCounty( ddmCounty* county );

    void updateStateNames();
    void updateCountyNames();

};

#endif // DDM_COUNTY_FILTER_MODEL_H
