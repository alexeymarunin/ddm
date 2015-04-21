#include "widgets/ddmMaxInOutNeighborFilterWidget.h"
#include "filters/ddmMaxInOutNeighborFilter.h"
#include "ui_ddmMaxInOutNeighborFilterWidget.h"

/**
 * Констуктор класса
 *
 * @param   filter Фильтр, к которому относится виджет
 * @param   parent Родитель (владелец) виджета
 * @author  Марунин А.В.
 * @since   2.10
 */
ddmMaxInOutNeighborFilterWidget::ddmMaxInOutNeighborFilterWidget( ddmMaxInOutNeighborFilter* filter, QWidget* parent ) : ddmFilterWidget( filter, parent )
{
    this->ui = new Ui::ddmMaxInOutNeighborFilterWidget;
    this->ui->setupUi( this );

}

/**
 * Задает тип соседства
 *
 * @param   type ТЦелое число типа ddmNeighborType
 * @author  Марунин А.В.
 * @since   2.10
 * @see     ddmNeighborType
 */
void ddmMaxInOutNeighborFilterWidget::setNeighborType( ddmNeighborType type )
{
    this->neighborTypeCombo()->setCurrentIndex( (int)type );
}

/**
 * Возвращает тип соседства
 * @return  Целое число типа ddmNeighborType
 * @author  Марунин А.В.
 * @since   2.10
 * @see     ddmNeighborType
 */
ddmNeighborType ddmMaxInOutNeighborFilterWidget::neighborType() const
{
    return (ddmNeighborType)this->neighborTypeCombo()->currentIndex();
}

/**
 * Доступ к выпадающему списку с типами соседей
 *
 * @return  Указатель на объект типа QComboBox
 * @author  Марунин А.В.
 * @since   2.10
 */
QComboBox* ddmMaxInOutNeighborFilterWidget::neighborTypeCombo() const
{
    return const_cast<ddmMaxInOutNeighborFilterWidget*>( this )->ui->comboType;
}

/**
 * Устанавливает/сбрасывает переключатель принадлежности соседей одному штату
 *
 * @param   checked true, если необходимо, чтобы графства принадлежали одному штату
 * @author  Марунин А.В.
 * @since   2.10
 */
void ddmMaxInOutNeighborFilterWidget::checkSameState( bool checked )
{
    this->sameStateCheckbox()->setChecked( checked );
}

/**
 * Возвращает флаг принадлежности соседей к одному штату
 *
 * @return  true, если нужно выбирать соседей только из одного штата
 * @author  Марунин А.В.
 * @since   2.10
 */
bool ddmMaxInOutNeighborFilterWidget::sameStateChecked() const
{
    return this->sameStateCheckbox()->isChecked();
}

/**
 * Доступ к переключателю графств только из одного штата
 *
 * @return  Указатель на объект типа QCheckBox
 * @author  Марунин А.В.
 * @since   2.10
 */
QCheckBox* ddmMaxInOutNeighborFilterWidget::sameStateCheckbox() const
{
    return const_cast<ddmMaxInOutNeighborFilterWidget*>( this )->ui->checkSameState;
}

/**
 * Формирует список графств
 *
 * @param   countyNames Объект типа QStringList - список имен графств
 * @author  Марунин А.В.
 * @since   2.10
 */
void ddmMaxInOutNeighborFilterWidget::setCounties( const QStringList& countyNames )
{
    this->countryList()->clear();
    this->countryList()->addItems( countyNames );
}

/**
 * Задает текущее графство в списке
 *
 * @param   countyName Название графства
 * @author  Марунин А.В.
 * @since   2.10
 */
void ddmMaxInOutNeighborFilterWidget::setCurrentCounty( const QString& countyName )
{
    QList<QListWidgetItem*> items = this->countryList()->findItems( countyName, Qt::MatchFixedString );
    if ( items.length() > 0 )
    {
        this->countryList()->setCurrentItem( items.first(), QItemSelectionModel::SelectCurrent );
    }
}

/**
 * Возвращает текущее выбранное в списке графство
 *
 * @return  Название текущего графства
 * @author  Марунин А.В.
 * @since   2.10
 */
QString ddmMaxInOutNeighborFilterWidget::currentCounty() const
{
    QListWidgetItem* currentItem = this->countryList()->currentItem();
    return currentItem ? currentItem->text() : QString( "" );
}

/**
 * Доступ к списку найденных графств
 *
 * @return  Указатель на объект типа QListWidget
 * @author  Марунин А.В.
 * @since   2.10
 */
QListWidget* ddmMaxInOutNeighborFilterWidget::countryList() const
{
    return this->ui->listCounties;
}

/**
 * Деструктор класса
 *
 * @author  Марунин А.В.
 * @since   2.10
 */
ddmMaxInOutNeighborFilterWidget::~ddmMaxInOutNeighborFilterWidget()
{
    delete this->ui;
}
