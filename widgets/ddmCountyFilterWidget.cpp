#include "widgets/ddmCountyFilterWidget.h"
#include "filters/ddmCountyFilter.h"
#include "ui_ddmCountyFilterWidget.h"


ddmCountyFilterWidget::ddmCountyFilterWidget( ddmCountyFilter* countyFilter, QWidget* parent ) : ddmFilterWidget( countyFilter, parent )
{
    this->ui = new Ui::ddmCountyFilterWidget;

    this->ui->setupUi( this );
    this->ui->gridLayout->setContentsMargins( 0, 0, 0, 0 );
    this->setContentsMargins( 0, 0, 0, 0 );

    QObject::connect( this->comboState(),  SIGNAL( currentIndexChanged(const QString&) ), this, SIGNAL( changedState() ) );
    QObject::connect( this->comboCounty(), SIGNAL( currentIndexChanged(const QString&) ), this, SIGNAL( changedCounty() ) );

    QObject::connect( this->comboState(),  SIGNAL( currentIndexChanged(const QString&) ), this, SIGNAL( changed() ) );
    QObject::connect( this->comboCounty(), SIGNAL( currentIndexChanged(const QString&) ), this, SIGNAL( changed() ) );
}

QString ddmCountyFilterWidget::currentState() const
{
    return this->comboState()->currentIndex()>= 0 ? this->comboState()->currentText() : QString();
}

void ddmCountyFilterWidget::setCurrentState( const QString& state )
{
    if ( this->comboState()->findText( state ) )
    {
        this->comboState()->setCurrentText( state );
    }
}

QString ddmCountyFilterWidget::currentCounty() const
{
    return this->comboCounty()->currentIndex()>= 0 ? this->comboCounty()->currentText() : QString();
}

void ddmCountyFilterWidget::setCurrentCounty( const QString& county )
{
    if ( this->comboCounty()->findText( county ) )
    {
        this->comboCounty()->setCurrentText( county );
    }
}

void ddmCountyFilterWidget::setStateNames( const QStringList& items )
{
    if ( this->comboState()->count() == 0 )
    {
        this->comboState()->addItems( items );
    }
}

void ddmCountyFilterWidget::setCountyNames( const QStringList& names )
{
    if ( !names.isEmpty() )
    {
        QComboBox* combo = this->comboCounty();
        combo->clear();
        combo->addItems( names );
    }
}

QComboBox* ddmCountyFilterWidget::comboState() const
{
    return this->ui->comboState;
}

QComboBox* ddmCountyFilterWidget::comboCounty() const
{
    return this->ui->comboCounty;
}

/**
 * @brief ddmCountyFilterWidget::~ddmCountyFilterWidget
 */
ddmCountyFilterWidget::~ddmCountyFilterWidget()
{
    delete this->ui;
}
