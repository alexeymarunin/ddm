#ifndef DDM_COUNTY_H
#define DDM_COUNTY_H


#include "base/ddmMapObject.h"

class ddmState;
class ddmBoundary;

/**
 * Класс ddmBoundary описывает графство
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
class ddmCounty : public ddmMapObject
{
    Q_OBJECT

public:
    ddmCounty( int id = 0, const QString& geographicName = "", QObject* parent = 0 );
    ddmCounty( const QSqlRecord& record, QObject* parent = 0 );

    Q_PROPERTY( QString      name       READ geographicName )
    Q_PROPERTY( QVariantList boundaries READ boundaries     )
    Q_PROPERTY( QVariantMap  center     READ center         )

    Q_PROPERTY( int     f_out_sum   READ f_out_sum )
    Q_PROPERTY( double  f_out_mid   READ f_out_mid )
    Q_PROPERTY( int     f_in_sum    READ f_in_sum  )
    Q_PROPERTY( double  f_in_mid    READ f_in_mid  )
    Q_PROPERTY( double  f_mid       READ f_mid     )

    Q_PROPERTY( QString fillColor       MEMBER m_fillColor      NOTIFY repaint )
    Q_PROPERTY( double  fillOpacity     MEMBER m_fillOpacity    NOTIFY repaint )
    Q_PROPERTY( QString strokeColor     MEMBER m_strokeColor    NOTIFY repaint )
    Q_PROPERTY( int     strokeWeight    MEMBER m_strokeWeight   NOTIFY repaint )
    Q_PROPERTY( double  strokeOpacity   MEMBER m_strokeOpacity  NOTIFY repaint )

    void         addBoundary( ddmBoundary* boundary );
    ddmBoundary* boundary( int id ) const;
    QVariantList boundaries() const;

    // Значения базовых свойств
    QString     geographicName() const;
    QVariantMap center() const;
    ddmState*   state() const;

    // Значения трений
    int    f_out_sum() const;
    double f_out_mid() const;
    int    f_in_sum()  const;
    double f_in_mid()  const;
    double f_mid()     const;

    virtual ~ddmCounty();

Q_SIGNALS:

    void clicked( double x, double y );
    void mouseover();
    void mouseout();
    void mousemove( double x, double y );

    void repaint();

protected Q_SLOTS:

    void slotMouseover();
    void slotMouseout();

protected:

    QString m_geographicName;

    int     m_f_out_sum;
    double  m_f_out_mid;
    int     m_f_in_sum;
    double  m_f_in_mid;
    double  m_f_mid;

    QString m_fillColor;
    double  m_fillOpacity;
    QString m_strokeColor;
    int     m_strokeWeight;
    double  m_strokeOpacity;

    virtual void create( const QSqlRecord& record );
    void create( int id, const QString& geographicName,
                 int f_out_sum = 0, double f_out_mid = 0.0,
                 int f_in_sum = 0, double f_in_mid = 0.0, double f_mid = 0.0 );

};

#endif // DDM_COUNTY_H
