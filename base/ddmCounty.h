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
    Q_PROPERTY( QVariantMap  bounds     READ bounds         )

    Q_PROPERTY( int     population  READ population )
    Q_PROPERTY( int     in_sum      READ in_sum     )
    Q_PROPERTY( int     out_sum     READ out_sum    )
    Q_PROPERTY( int     delta       READ delta      )

    Q_PROPERTY( int     f_out_sum   READ f_out_sum  )
    Q_PROPERTY( double  f_out_mid   READ f_out_mid  )
    Q_PROPERTY( int     f_in_sum    READ f_in_sum   )
    Q_PROPERTY( double  f_in_mid    READ f_in_mid   )
    Q_PROPERTY( double  f_mid       READ f_mid      )

    Q_PROPERTY( QString fillColor       MEMBER m_fillColor      NOTIFY repaint )
    Q_PROPERTY( double  fillOpacity     MEMBER m_fillOpacity    NOTIFY repaint )
    Q_PROPERTY( QString strokeColor     MEMBER m_strokeColor    NOTIFY repaint )
    Q_PROPERTY( int     strokeWeight    MEMBER m_strokeWeight   NOTIFY repaint )
    Q_PROPERTY( double  strokeOpacity   MEMBER m_strokeOpacity  NOTIFY repaint )

    Q_PROPERTY( bool visible READ visible WRITE setVisible )

    void         addBoundary( ddmBoundary* boundary );
    ddmBoundary* boundary( int id ) const;
    QVariantList boundaries() const;

    // Значения базовых свойств
    QString     geographicName() const;
    QVariantMap center() const;
    ddmState*   state() const;
    QVariantMap bounds() const;

    int population() const;
    int in_sum() const;
    int out_sum() const;
    int delta() const;

    // Значения трений
    int    f_out_sum() const;
    double f_out_mid() const;
    int    f_in_sum()  const;
    double f_in_mid()  const;
    double f_mid()     const;

    bool visible() const;
    void setVisible( bool visible );
    void show();
    void hide();

    virtual ~ddmCounty();

Q_SIGNALS:

    void clicked( double x, double y );
    void mouseover();
    void mouseout();
    void mousemove( double x, double y );

    void shown();
    void hidden();
    void repaint();

protected Q_SLOTS:

    void slotMouseover();
    void slotMouseout();

protected:

    QString m_geographicName;

    int     m_population;
    int     m_in_sum;
    int     m_out_sum;
    int     m_delta;

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

    bool    m_visible;

    virtual void create( const QSqlRecord& record );
    void create( int id, const QString& geographicName,
                 int population = 0, int in_sum = 0, int out_sum = 0, int delta = 0,
                 int f_out_sum = 0, double f_out_mid = 0.0,
                 int f_in_sum = 0, double f_in_mid = 0.0, double f_mid = 0.0 );

};

#endif // DDM_COUNTY_H
