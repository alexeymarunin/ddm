#ifndef DDMOBJECTMODEL_H
#define DDMOBJECTMODEL_H

#include<QObject>
#include<QString>


class ddmObjectModel: public QObject
{
    Q_OBJECT

public:

    ddmObjectModel();

    virtual ~ddmObjectModel();

    void setCurentState(  const QString state );

    void setCurentCounty( const QString county );

    QString CurentState();

    QString CurentCounty();

signals:
    void updateState( const QString& state );

    void updateCounty( const QString& county );


private:
   QString m_state;
   QString m_county;
};

#endif // DDMOBJECTMODEL_H
