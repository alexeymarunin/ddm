#ifndef DDMOBJECTMODEL_H
#define DDMOBJECTMODEL_H

#include<QObject>
#include <QtSql>
#include<QString>
#include <QStringList>


class ddmObjectModel: public QObject
{
    Q_OBJECT

public:

    ddmObjectModel();

    virtual ~ddmObjectModel();

    void setCurentState(  const QString state );

    void setCurentCounty( const QString county );

    bool openDB(  const QString& fullPath );

    QString CurentState();

    int GetCurentStateId();

    QString CurentCounty();

    bool isOpen();

    void getStatesList( QStringList& states );

    void getCountiesList( QStringList& counties, const QString& state );




signals:
    void updateState( const QString& state );

    void updateCounty( const QString& county, int state_id );


private:
   QString m_state;
   QString m_county;

   QSqlDatabase m_dbase;
};

#endif // DDMOBJECTMODEL_H
