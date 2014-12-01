#ifndef DDMOBJECTMODEL_H
#define DDMOBJECTMODEL_H

#include<QString>

class ddmObjectModel
{
public:
    ddmObjectModel();

    ~ddmObjectModel();

    void setCurentState(  const QString state );

    void setCurentCounty( const QString county );

    QString CurentState();

    QString CurentCounty();

private:
   QString m_state;
   QString m_county;
};

#endif // DDMOBJECTMODEL_H
