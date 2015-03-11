#ifndef DDM_MAP_OBJECT_H
#define DDM_MAP_OBJECT_H

#include <QString>
#include <QColor>
#include <QSqlRecord>
#include <QPointF>

#include "base/ddmObject.h"

/**
 * Класс ddmMapObject является базовым для других классов,
 * использующихся в JavaScript
 * Определяет базовые свойства и сигналы
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
class ddmMapObject : public ddmObject
{
    Q_OBJECT

public:
    ddmMapObject( int id = 0, QObject* parent = 0 );

    Q_PROPERTY( int id READ id )

    int id() const;

    virtual ~ddmMapObject();

Q_SIGNALS:

    void visibleChanged();

protected:
    int     m_id;

    virtual void create( const QSqlRecord& record ) = 0;

};

#endif // DDM_MAP_OBJECT_H
