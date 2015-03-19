#include "widgets/ddmBaseCountyFilterWidget.h"
#include "filters/ddmBaseCountyFilter.h"
#include "ui_ddmBaseCountyFilterWidget.h"

/**
 * Конструктор класса
 *
 * @param   filter Фильтр, к которому относится виджет
 * @param   parent Родитель (владелец) виджета
 * @author  Марунин А.В.
 * @since   2.8
 */
ddmBaseCountyFilterWidget::ddmBaseCountyFilterWidget( ddmBaseCountyFilter* filter, QWidget* parent ) : ddmFilterWidget( filter, parent )
{
    this->ui = new Ui::ddmBaseCountyFilterWidget;

    this->ui->setupUi( this );
    this->ui->gridLayout->setContentsMargins( 0, 0, 0, 0 );
    this->setContentsMargins( 0, 0, 0, 0 );

    QObject::connect( this->comboState(),  SIGNAL( currentIndexChanged(const QString&) ), this, SIGNAL( changedState() ) );
    QObject::connect( this->comboCounty(), SIGNAL( currentIndexChanged(const QString&) ), this, SIGNAL( changedCounty() ) );

    QObject::connect( this->comboState(),  SIGNAL( currentIndexChanged(const QString&) ), this, SIGNAL( changed() ) );
    QObject::connect( this->comboCounty(), SIGNAL( currentIndexChanged(const QString&) ), this, SIGNAL( changed() ) );
}

/**
 * Возвращает название текущего штата
 *
 * @return  Объект типа QString
 * @author  Марунин А.В.
 * @since   2.8
 */
QString ddmBaseCountyFilterWidget::currentState() const
{
    return this->comboState()->currentIndex()>= 0 ? this->comboState()->currentText() : QString();
}

/**
 * Задает текущий штат
 *
 * @param   state Имя штата
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmBaseCountyFilterWidget::setCurrentState( const QString& state )
{
    if ( this->comboState()->findText( state ) )
    {
        this->comboState()->setCurrentText( state );
    }
}

/**
 * Возвращает название текущего графства
 *
 * @return  Объект типа QString
 * @author  Марунин А.В.
 * @since   2.8
 */
QString ddmBaseCountyFilterWidget::currentCounty() const
{
    return this->comboCounty()->currentIndex()>= 0 ? this->comboCounty()->currentText() : QString();
}

/**
 * @brief ddmBaseCountyFilterWidget::setCurrentCounty
 * @param county
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmBaseCountyFilterWidget::setCurrentCounty( const QString& county )
{
    if ( this->comboCounty()->findText( county ) )
    {
        this->comboCounty()->setCurrentText( county );
    }
}

/**
 * Задает список имен штатов
 *
 * @param   items Объект типа QStringList
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmBaseCountyFilterWidget::setStateNames( const QStringList& items )
{
    if ( this->comboState()->count() == 0 )
    {
        this->comboState()->addItems( items );
    }
}

/**
 * Задает список имен графств для текущего штата
 *
 * @param   names Объект типа QStringList
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmBaseCountyFilterWidget::setCountyNames( const QStringList& names )
{
    if ( !names.isEmpty() )
    {
        QComboBox* combo = this->comboCounty();
        combo->clear();
        combo->addItems( names );
    }
}

/**
 * Встраивает доп.панель в виджет
 *
 * Позволяет задавать виджет с доп.настройками фильтра
 *
 * @param   widget Встраиваемый виджет
 * @author  Марунин А.В.
 * @since   2.8
 */
void ddmBaseCountyFilterWidget::insertWidget( QWidget* widget )
{
    this->ui->verticalLayout->insertWidget( 4, widget );
}

/**
 * Доступ к выпадающему списку штатов
 *
 * @return  Указатель на объект типа QComboBox
 * @author  Марунин А.В.
 * @since   2.8
 */
QComboBox* ddmBaseCountyFilterWidget::comboState() const
{
    return this->ui->comboState;
}

/**
 * Доступ к выпадающему списку графств
 *
 * @return  Указатель на объект типа QComboBox
 * @author  Марунин А.В.
 * @since   2.8
 */
QComboBox* ddmBaseCountyFilterWidget::comboCounty() const
{
    return this->ui->comboCounty;
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.8
 */
ddmBaseCountyFilterWidget::~ddmBaseCountyFilterWidget()
{
    delete this->ui;
}
