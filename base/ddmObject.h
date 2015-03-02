#ifndef DDM_OBJECT_H
#define DDM_OBJECT_H

#include <QObject>
#include <QVariantList>

/**
 * Класс ddmObject является базовым классом во всем проекте
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
class ddmObject : public QObject
{
    Q_OBJECT

public:

    explicit ddmObject( QObject* parent = 0 );

    QVariantList findChilds( const QString& className ) const;

    QObject* findChildById( int id, const QString& className = "" ) const;

    int childCount( const QString& className ) const;

    QString className() const;
    bool isClass( const QString& className ) const;

    virtual ~ddmObject();

};

#endif // DDM_OBJECT_H
