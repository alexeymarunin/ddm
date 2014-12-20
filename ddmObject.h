#ifndef DDM_OBJECT_H
#define DDM_OBJECT_H

#include <QObject>

class ddmObject : public QObject
{
    Q_OBJECT
public:
    explicit ddmObject( int id = 0, QObject* parent = 0 );

    void setId( int id );
    int id() const;

    virtual ~ddmObject();

signals:

public slots:

protected:
    int m_id;
};

#endif // DDM_OBJECT_H
