#ifndef DDM_STATE_H
#define DDM_STATE_H

#include "ddmObject.h"

class ddmModel;

class ddmState : public ddmObject
{
    Q_OBJECT
public:
    explicit ddmState( int id = 0, ddmModel* model = 0 );

    void setGeographicName( const QString& name );
    QString geographicName() const;

    virtual ~ddmState();

signals:

public slots:

private:
    QString m_geographicName;

};

#endif // DDM_STATE_H
