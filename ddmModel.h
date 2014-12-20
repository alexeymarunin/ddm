#ifndef DDM_MODEL_H
#define DDM_MODEL_H

#include <QObject>
#include <QtSql>
#include <QStringList>

#include "ddmState.h"
#include "ddmCounty.h"

class QStringList;

typedef QMap<int, ddmState*> ddmStateMap;
typedef QMap<int, ddmCounty*> ddmCountyMap;

class ddmModel : public QObject
{
    Q_OBJECT

public:

    ddmModel( QObject* parent = 0 );

    bool openDatabase( const QString& fullPath );

    ddmStateMap& states() const;
    ddmCountyMap& counties() const;

    ddmState* currentState() const;
    ddmCounty* currentCounty() const;
    void setCurrentState( int id );
    void setCurrentState( const QString& stateName );
    void setCurrentCounty( int id );
    void setCurrentCounty( const QString& countyName );

    QStringList stateNames() const;
    QStringList countyNames() const;

    virtual ~ddmModel();

signals:

    void changedState( ddmState* state );
    void changedCounty( ddmCounty* county );


private:
    typedef QMap<ddmState*, QList<ddmCounty*> > ddmStateCountyMap;

    ddmStateMap         m_states;
    ddmCountyMap        m_counties;
    ddmStateCountyMap   m_stateCounties;
    ddmState*           m_currentState;
    ddmCounty*          m_currentCounty;

    QSqlDatabase        m_db;

    void loadStates();
    void loadCounties();
    void loadBoundaries();

    void setCurrentState( ddmState* state );
    void setCurrentCounty( ddmCounty* county );
};

#endif // DDM_MODEL_H
