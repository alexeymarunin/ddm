#include <QResizeEvent>
#include <QWebFrame>
#include <QComboBox>
#include <QVBoxLayout>
#include <QMenu>

#include "ddmMapView.h"
#include "widgets/ddmCentralWidget.h"
#include "ui_ddmCentralWidget.h"
#include "ddmSettings.h"
#include "filters/ddmFilter.h"

#include "ddmInfoLogger.h"

/**
 * Конструктор класса
 *
 * @param   parent Владелец виджета
 * @author  Марунин А.В.
 * @since   2.0
 */
ddmCentralWidget::ddmCentralWidget( QWidget* parent ) : QWidget( parent ),
    m_currentFilter( NULL )
{
    this->ui = new Ui::ddmCentralWidget;
    this->ui->setupUi( this );
    ddmInfoLogger& logger = ddmInfoLogger::instance();
    logger.setOutputWidget( ui->m_InfoLog );

    ui->m_InfoLog->setContextMenuPolicy( Qt::CustomContextMenu );
    connect( ui->m_InfoLog , SIGNAL( customContextMenuRequested( const QPoint & )),
               this, SLOT( ShowContextMenu( const QPoint &) ) );


    this->loadSettings();

    QObject::connect( this->comboFilter(), SIGNAL( currentIndexChanged(int) ), this, SLOT( slotChangedFilter(int) ) );

}

void ddmCentralWidget::appendFilter( const QString& title, ddmFilter* filter )
{
    QComboBox* comboFilter = this->comboFilter();
    comboFilter->addItem( title, QVariant::fromValue( filter ) );

}

void ddmCentralWidget::setCurrentFilter( int index )
{
    int currentIndex = this->comboFilter()->currentIndex();
    if ( currentIndex != index )
    {
        this->comboFilter()->setCurrentIndex( index );
    }
    else
    {
        this->slotChangedFilter( index );
    }
}

/**
 * Задает текущий фильтр по названию класса
 *
 * @param className Имя класса фильтра
 * @author  Марунин А.В.
 * @since   2.6
 */
void ddmCentralWidget::setCurrentFilter( const QString& className )
{
    if ( !className.isEmpty() )
    {
        QComboBox* comboFilter = this->comboFilter();
        int filterCount = comboFilter->count();
        for ( int i = 0; i < filterCount; i++ )
        {
            ddmFilter* filter = comboFilter->itemData( i ).value<ddmFilter*>();
            if ( filter->metaObject()->className() ==  className )
            {
                this->setCurrentFilter( i );
                break;
            }
        }
    }
}

QComboBox* ddmCentralWidget::comboFilter() const
{
    return this->ui->comboFilter;
}

void ddmCentralWidget::slotChangedFilter( int index )
{
    QComboBox* comboFilter = this->comboFilter();
    ddmFilter* filter = comboFilter->itemData( index ).value<ddmFilter*>();

    if ( this->currentFilter() != filter )
    {
        if ( this->currentFilter() )
        {
            this->currentFilter()->deactivate();
            this->m_currentFilter = NULL;
        }
        if ( filter )
        {
            this->m_currentFilter = filter;
            filter->activate();
        }
    }
}

ddmFilter* ddmCentralWidget::currentFilter() const
{
    return this->m_currentFilter;
}

void ddmCentralWidget::resizeEvent( QResizeEvent* event )
{
    QSize size = event->size();
    if ( this->currentFilter() )
    {
        this->currentFilter()->mapView()->resize( size.width(), size.height() );
    }
}

void ddmCentralWidget::reload()
{
    if ( this->currentFilter() )
    {
        this->currentFilter()->mapView()->reload();
    }
}

void ddmCentralWidget::ShowContextMenu( const QPoint& point )
{
    QMenu contextMenu(tr( "Context menu" ), ui->m_InfoLog );
    ui->m_clearTextAction->setParent( ui->m_InfoLog );
    connect( ui->m_clearTextAction, SIGNAL( triggered() ), this,
             SLOT( slotClearText() ), Qt::UniqueConnection );
    contextMenu.addAction( ui->m_clearTextAction );
    contextMenu.exec( ui->m_InfoLog->mapToGlobal( point ) );

}


void ddmCentralWidget::slotClearText()
{
    ui->m_InfoLog->clear();
}


void ddmCentralWidget::increaseZoom()
{
    if ( this->currentFilter() )
        this->currentFilter()->mapView()->increaseZoomLevel();
}


void ddmCentralWidget::decreaseZoom()
{
    if ( this->currentFilter() )
        this->currentFilter()->mapView()->decreaseZoomLevel();
}

QVBoxLayout* ddmCentralWidget::widgetLayout() const
{
    return this->ui->widgetLayout;
}

QVBoxLayout* ddmCentralWidget::mapLayout() const
{
    return this->ui->mapLayout;
}

/**
 * Загружает настройки виджета
 *
 * @author  Марунин А.В.
 * @since   2.6
 */
void ddmCentralWidget::loadSettings()
{
   ddmSettings* settings = ddmSettings::instance();
   QByteArray splitterState = settings->value( "panel/state", QByteArray() ).toByteArray();
   if ( splitterState.size() )
   {
       this->ui->splitter->restoreState( splitterState );
   }
}

/**
 * Сохраняет настройки виджета
 *
 * @author  Марунин А.В.
 * @since   2.6
 */
void ddmCentralWidget::saveSettings()
{
    ddmSettings* settings = ddmSettings::instance();

    if ( this->currentFilter() )
    {
        QString lastFilter = this->currentFilter()->metaObject()->className();
        settings->setValue( "panel/filter", lastFilter );
    }
    settings->setValue( "panel/state", this->ui->splitter->saveState() );
}

ddmCentralWidget::~ddmCentralWidget()
{
    this->saveSettings();

    delete this->ui;
}
