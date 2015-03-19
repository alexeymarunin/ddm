#include "ui_ddmEmptyFilterWidget.h"
#include "filters/ddmEmptyFilter.h"
#include "widgets/ddmEmptyFilterWidget.h"

ddmEmptyFilterWidget::ddmEmptyFilterWidget( ddmEmptyFilter* filter, QWidget* parent ) : ddmFilterWidget( filter, parent )
{
    this->ui = new Ui::ddmEmptyFilterWidget;

    this->ui->setupUi( this );
    ui->gridLayout->setContentsMargins( 0, 0, 0, 0 );
    this->setContentsMargins( 0, 0, 0, 0 );
}

ddmEmptyFilterWidget::~ddmEmptyFilterWidget()
{
    delete this->ui;
}
