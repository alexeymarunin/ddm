#include "base/ddmCounty.h"
#include "models/ddmMaxInOutNeighborFilterModel.h"
#include "widgets/ddmMaxInOutNeighborFilterWidget.h"
#include "filters/ddmMaxInOutNeighborFilter.h"
#include "ddmSettings.h"
#include "ddmInfoLogger.h"


/**
 * Конструктор класса
 *
 * @param   parent Родитель (владелец) фильтра
 * @author  Марунин А.В.
 * @since   2.10
 */
ddmMaxInOutNeighborFilter::ddmMaxInOutNeighborFilter( QObject* parent ) : ddmFilter( parent )
{
}

/**
 * Выполняет инициализацию фильтра при создании
 *
 * @author  Марунин А.В.
 * @since   2.10
 */
void ddmMaxInOutNeighborFilter::setup()
{
    ddmMaxInOutNeighborFilterModel* model = new ddmMaxInOutNeighborFilterModel( this );
    ddmMaxInOutNeighborFilterWidget* widget = new ddmMaxInOutNeighborFilterWidget( this );
    ddmInfoLogger& logger = ddmInfoLogger::instance();
    logger.writeInfo( "Выбран фильтр: Графства-книппели" );

    this->m_model = model;
    this->m_widget = widget;

    // Загружаем настройки
    this->loadSettings();

    QObject::connect( widget->neighborTypeCombo(), SIGNAL( currentIndexChanged(int) ), this, SLOT( slotWidgetChanged() ) );
    QObject::connect( widget->sameStateCheckbox(), SIGNAL( clicked() ), this, SLOT( slotWidgetChanged() ) );

    //QObject::connect( this->countryList(),  SIGNAL( itemSelectionChanged() ), this, SLOT( changeCounty() ) );

    model->reloadData();
    // Обязательно вызываем метод из базового класса!
    ddmFilter::setup();

}

/**
 * Синхронизирует данные между виджетом и моделью
 *
 * Если параметр fromWidget = true (по умолчанию), то данные с виджета заносятся в модель
 *
 * @param   Флаг, который указывает направление синхронизации.
 * @author  Марунин А.В.
 * @since   2.10
 */
void ddmMaxInOutNeighborFilter::updateData( bool fromWidget )
{
    if ( this->valid() )
    {
        ddmMaxInOutNeighborFilterModel* model = this->model_cast<ddmMaxInOutNeighborFilterModel>();
        ddmMaxInOutNeighborFilterWidget* widget = this->widget_cast<ddmMaxInOutNeighborFilterWidget>();
        if ( fromWidget )
        {
            model->setNeighborOptions( widget->neighborType(), widget->sameStateChecked() );
            qDebug() << widget->neighborType();
        }
        else
        {
            widget->setNeighborType( model->neighborType() );
            widget->checkSameState( model->sameState() );
            widget->setCounties( model->countyNames() );
            this->saveSettings();
        }
    }
}

/**
 * Обновляет выделенные графства
 *
 * @author  Марунин А.В.
 * @since   2.10
 */
void ddmMaxInOutNeighborFilter::updateSelection()
{
    this->resetSelection();

    ddmMaxInOutNeighborFilterModel* model = this->model_cast<ddmMaxInOutNeighborFilterModel>();
    ddmInfoLogger& logger = ddmInfoLogger::instance();

    switch ( model->neighborType() )
    {
    case NT_FIRST_NEIGHBOR:
        logger.writeInfo( QObject::tr( "Отображаются графства-книппели с соседями 1-го порядка" ) );
        break;
    case NT_SECOND_NEIGHBOR:
        logger.writeInfo( QObject::tr( "Отображаются графства-книппели с соседями второго порядка" ) );
        break;
    case NT_FAR_NEIGHBOR:
        logger.writeInfo( QObject::tr( "Отображаются графства-книппели с разрозненными соседями" ) );
        break;
    default:
        logger.writeInfo( QObject::tr( "Отображаются все графства-книппели" ) );
    }

    QVariantList counties = model->counties();
    QVariantList neighbors = model->neighbors();

    bool needUpdate = false;
    foreach ( QVariant obj, counties )
    {
        ddmCounty* county = obj.value<ddmCounty*>();
        county->show();
        needUpdate = true;
    }
    foreach ( QVariant obj, neighbors )
    {
        ddmCounty* county = obj.value<ddmCounty*>();
        county->show();
        needUpdate = true;
    }

    if ( needUpdate )
    {
        Q_EMIT selectionUpdated();
    }

    int totalCount = counties.size() + neighbors.size();
    logger.writeInfo( QObject::tr( "Найдено %1 графств(а)" ).arg( totalCount ) );
}


/**
 * Загружает настройки фильтра
 *
 * @author  Марунин А.В.
 * @since   2.10
 */
void ddmMaxInOutNeighborFilter::loadSettings()
{
    ddmSettings* settings = ddmSettings::instance();

    QString selectCounty = settings->value( "ddmMaxInOutNeighborFilter/select_county", "" ).toString();
    ddmNeighborType neighborType = (ddmNeighborType)settings->value( "ddmMaxInOutNeighborFilter/neighbor_type", (int)NT_FIRST_NEIGHBOR ).toInt();
    bool sameState = settings->value( "ddmMaxInOutNeighborFilter/same_state", true ).toBool();

    ddmMaxInOutNeighborFilterModel* model  = this->model_cast<ddmMaxInOutNeighborFilterModel>();
    model->setNeighborOptions( neighborType, sameState );
}

/**
 * Сохраняет настройки фильтра
 *
 * @author  Марунин А.В.
 * @since   2.10
 */
void ddmMaxInOutNeighborFilter::saveSettings()
{
    ddmSettings* settings = ddmSettings::instance();

    //ddmMaxInOutNeighborFilterModel* model  = this->model_cast<ddmMaxInOutNeighborFilterModel>();
    ddmMaxInOutNeighborFilterWidget* widget = this->widget_cast<ddmMaxInOutNeighborFilterWidget>();
    settings->setValue( "ddmMaxInOutNeighborFilter/select_county", widget->currentCounty() );
    settings->setValue( "ddmMaxInOutNeighborFilter/neighbor_type", (int)widget->neighborType() );
    settings->setValue( "ddmMaxInOutNeighborFilter/same_state", widget->sameStateChecked() );
}

/**
 * Обработчик изменений в виджете фильтра
 *
 * @author  Марунин А.В.
 * @since   2.10
 */
void ddmMaxInOutNeighborFilter::slotWidgetChanged()
{
    this->apply();
}


/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.10
 */
ddmMaxInOutNeighborFilter::~ddmMaxInOutNeighborFilter()
{
}


