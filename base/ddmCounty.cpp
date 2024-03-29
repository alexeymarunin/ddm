#include <QDebug>
#include "base/ddmCounty.h"
#include "base/ddmState.h"
#include "base/ddmBoundary.h"


/**
 * Конструктор по умолчанию
 *
 * @param   id Уникальный идентификатор графства
 * @param   geographicName Географическое название графства (названия + штат)
 * @param   parent Родитель графства (обычно ddmState)
 * @author  Марунин А.В.
 * @since   2.1
 */
ddmCounty::ddmCounty( int id, const QString& geographicName, QObject* parent ) : ddmMapObject( id, parent ),
    m_visible( false )
{
    this->create( id, geographicName );
}

/**
 * Конструктор
 *
 * @param   record SQL-запись, из которой будут взяты параметры графства
 * @param   parent Родитель графства (обычно ddmState)
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmCounty::ddmCounty( const QSqlRecord& record, QObject* parent ) : ddmMapObject( 0, parent ),
    m_visible( false )
{
    this->create( record );
}


/**
 * Инициализирует графство из записи SQL-запроса
 *
 * @param   record Запись SQL-запроса
 * @author  Марунин А.В.
 * @since   2.1
 */
void ddmCounty::create( const QSqlRecord& record )
{
    int     id              = record.value( "county_id" ).toInt();
    QString geographicName  = record.value( "county_name" ).toString();
    int     population      = record.value( "county_population" ).toInt();
    int     in_sum          = record.value( "county_in_sum" ).toInt();
    int     out_sum         = record.value( "county_out_sum" ).toInt();
    int     delta           = record.value( "county_delta" ).toInt();
    int     f_out_sum       = record.value( "county_f_out_sum" ).toInt();
    double  f_out_mid       = record.value( "county_f_out_mid" ).toDouble();
    int     f_in_sum        = record.value( "county_f_in_sum" ).toInt();
    double  f_in_mid        = record.value( "county_f_in_mid" ).toDouble();
    double  f_mid           = record.value( "county_f_mid" ).toDouble();

    this->create(
        id, geographicName,
        population, in_sum, out_sum, delta,
        f_out_sum, f_out_mid, f_in_sum, f_in_mid, f_mid
    );
}

/**
 * Инициализирует графство заданными параметрами
 *
 * @param   id Уникальный идентификатор графства
 * @param   geographicName Географическое название графства (названия + штат)
 * @param   f_out_sum Общее количество уехавших из графства
 * @param   f_out_mid Среднее трение по уехавшим
 * @param   f_in_sum Общее количество приехавших в графство
 * @param   f_in_mid Среднее трение по приехавшим
 * @param   f_mid Общее среднее трение графства
 * @author  Марунин А.В.
 * @since   2.1
 */
void ddmCounty::create( int id, const QString& geographicName,
                        int population, int in_sum, int out_sum, int delta,
                        int f_out_sum, double f_out_mid, int f_in_sum, double f_in_mid, double f_mid )
{
    this->m_id = id;
    this->m_geographicName = geographicName;

    this->m_population = population;
    this->m_in_sum = in_sum;
    this->m_out_sum = out_sum;
    this->m_delta = delta;

    this->m_f_out_sum = f_out_sum;
    this->m_f_out_mid = f_out_mid;
    this->m_f_in_sum = f_in_sum;
    this->m_f_in_mid = f_in_mid;
    this->m_f_mid = f_mid;

    this->m_fillColor = "#FF0000";
    this->m_fillOpacity = 0.35;
    this->m_strokeColor = "#FF0000";
    this->m_strokeWeight = 2;
    this->m_strokeOpacity = 0.7;

    QObject::connect( this, SIGNAL( mouseover() ), this, SLOT( slotMouseover() ) );
    QObject::connect( this, SIGNAL( mouseout()  ), this, SLOT( slotMouseout()  ) );

    this->setObjectName( QString( "county_%1" ).arg( this->m_id ) );
}

/**
 * Добавляет контур графства
 *
 * @param   boundary Указатель на контур
 * @see     boundary(), boundaries()
 * @author  Марунин А.В.
 * @since   2.1
 */
void ddmCounty::addBoundary( ddmBoundary* boundary )
{
    Q_ASSERT( boundary != NULL );
    /*
    if ( !this->boundary( boundary->id() ) )
    {
        boundary->setParent( this );
    }
    */
    boundary->setParent( this );
}

/**
 * Возвращает контур с заданным идентификатором
 *
 * @param   id Идентификатор контура
 * @return  Указатель на контур или NULL
 * @author  Марунин А.В.
 * @since   2.1
 */
ddmBoundary* ddmCounty::boundary( int id ) const
{
    return static_cast<ddmBoundary*>( this->findChildById( id, "ddmBoundary" ) );
}

/**
 * Возвращает список контуров графства
 *
 * @return  Объект типа QVariantMap, каждый элемент которого есть указатель на
 *          объект типа ddmBoundary, "завернутый" в QVariant
 * @author  Марунин А.В.
 * @since   2.1
 */
QVariantList ddmCounty::boundaries() const
{
    return this->findChilds( "ddmBoundary" );
}

/**
 * Возвращает штат, к которому принадлежит графство
 *
 * @return  Укатазатель на ddmState
 * @author  Марунин А.В.
 * @since   2.1
 */
ddmState* ddmCounty::state() const
{
    return qobject_cast<ddmState*>( this->parent() );
}

/**
 * Возвращает центр контура
 * Используется при передачи данных в JavaScript
 *
 * @return  Объект типа QVariantMap с двумя элементами - x и y
 * @author  Марунин А.В.
 * @since   2.1
 */
QVariantMap ddmCounty::center() const
{
    QVariantMap center;
    QVariantList boundaries = this->boundaries();
    if ( !boundaries.isEmpty() )
    {
        ddmBoundary* firstBoundary = this->boundaries().first().value<ddmBoundary*>();
        center = firstBoundary->center();
    }
    return center;
}

/**
 * Возвращает географическое имя графства (название + штат)
 *
 * @return  Географическое имя графства
 * @author  Марунин А.В.
 * @since   2.1
 */
QString ddmCounty::geographicName() const
{
    return this->m_geographicName;
}

/**
 * Возвращает число жителей графства
 *
 * @return  Целое число
 * @author  Марунин А.В.
 * @since   2.3
 */
int ddmCounty::population() const
{
    return this->m_population;
}

/**
 * Возвращает общее число жителей, приехавших в графство
 *
 * @return  Целое число
 * @author  Марунин А.В.
 * @since   2.3
 */
int ddmCounty::in_sum() const
{
    return this->m_in_sum;
}

/**
 * Возвращает общее число жителей, уехавших из графства
 *
 * @return  Целое число
 * @author  Марунин А.В.
 * @since   2.3
 */
int ddmCounty::out_sum() const
{
    return this->m_out_sum;
}

/**
 * Возвращает разницу между приехавшими и уехавшими
 *
 * @return  Целое число
 * @author  Марунин А.В.
 * @since   2.3
 */
int ddmCounty::delta() const
{
    return this->m_delta;
}

/**
 * Возвращает число уехавших из графства
 *
 * @return  Значение столбца f_out_sum из таблицы ddm_frictions
 * @author  Марунин А.В.
 * @since   2.1
 */
int ddmCounty::f_out_sum() const
{
    return this->m_f_out_sum;
}

/**
 * Возвращает среднее трение по уехавшим
 *
 * @return  Значение столбца f_out_mid из таблицы ddm_frictions
 * @author  Марунин А.В.
 * @since   2.1
 */
double ddmCounty::f_out_mid() const
{
    return this->m_f_out_mid;
}

/**
 * Возвращает число приехавших в графство
 *
 * @return  Значение столбца f_in_sum из таблицы ddm_frictions
 * @author  Марунин А.В.
 * @since   2.1
 */
int ddmCounty::f_in_sum() const
{
    return this->m_f_in_sum;
}

/**
 * Возвращает среднее трение по приехавшим
 *
 * @return  Значение столбца f_in_mid из таблицы ddm_frictions
 * @author  Марунин А.В.
 * @since   2.1
 */
double ddmCounty::f_in_mid() const
{
    return this->m_f_in_mid;
}

/**
 * Возвращает среднее значение трения графства
 *
 * @return  Значение поля f_mid из таблицы ddm_frictions
 * @author  Марунин А.В.
 * @since   2.1
 */
double ddmCounty::f_mid() const
{
    return this->m_f_mid;
}

/**
 * Возвращает видимость графства на карте
 *
 * @return  true, если графство отображается
 * @author  Марунин А.В.
 * @since   2.0
 */
bool ddmCounty::visible() const
{
    return this->m_visible;
}

/**
 * Изменяет видимость графства на карте.
 *
 * Если графство становится видимым, то испускается сигнал shown()
 * Если графство становится невидимым, то испускается сигнал hidden()
 *
 * @param   visible Флаг, определяющий, будет ли видно графство
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmCounty::setVisible( bool visible )
{
    if ( this->m_visible != visible )
    {
        this->m_visible = visible;
        // qDebug() << this->metaObject()->className() << this->id() << ( this->visible() ? "show" : "hide" );
        Q_EMIT ( this->visible() ? shown() : hidden() );
    }
}

/**
 * Показывает графство на карте
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmCounty::show()
{
    this->setVisible( true );
}

/**
 * Скрывает графство с карты
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
void ddmCounty::hide()
{
    this->setVisible( false );
}

/**
 * Слот для обработки сигнала mouseout
 * Вызывается, когда курсор входит в пределы графства
 *
 * @author  Марунин А.В.
 * @since   2.1
 */
void ddmCounty::slotMouseover()
{
    this->setProperty( "fillOpacity", 0.8 );
    this->setProperty( "strokeWeight", 3 );
    this->setProperty( "strokeOpacity", 0.8 );
}

/**
 * Слот для обработки сигнала mouseover
 * Вызывается, когда курсор покидает пределы графства
 *
 * @author  Марунин А.В.
 * @since   2.1
 */
void ddmCounty::slotMouseout()
{
    this->setProperty( "fillColor", "#FF0000" );
    this->setProperty( "fillOpacity", 0.35 );
    this->setProperty( "strokeColor", "#FF0000" );
    this->setProperty( "strokeWeight", 2 );
    this->setProperty( "strokeOpacity", 0.7 );
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.1
 */
ddmCounty::~ddmCounty()
{
}

