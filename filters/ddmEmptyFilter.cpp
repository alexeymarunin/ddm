#include "models/ddmEmptyFilterModel.h"
#include "widgets/ddmEmptyFilterWidget.h"
#include "filters/ddmEmptyFilter.h"

ddmEmptyFilter::ddmEmptyFilter( QObject* parent ) : ddmFilter( parent )
{
}

void ddmEmptyFilter::setup()
{
    ddmEmptyFilterModel* model = new ddmEmptyFilterModel( this );
    ddmEmptyFilterWidget* widget = new ddmEmptyFilterWidget( this );

    this->m_model = model;
    this->m_widget = widget;

    // Обязательно вызываем метод из базового класса!
    ddmFilter::setup();

    this->apply();
}

void ddmEmptyFilter::slotMapLoaded()
{
    this->setMapCenter( 44.4024, -100.459 );
}

ddmEmptyFilter::~ddmEmptyFilter()
{
}

