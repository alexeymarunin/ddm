#ifndef DDMFILTER
#define DDMFILTER

class QWidget;

class ddmMapView;
class ddmModel;

class ddmFilter
{
public:
    virtual void updateData() = 0;

    virtual void visualize();

    virtual QWidget* getWidget() = 0;

    ddmModel* model();

    ddmMapView* mapView();

protected:
    ddmFilter( ddmModel* model, ddmMapView* view );

private:
    ddmMapView* m_mapView;
    ddmModel* m_model;
};

#endif // DDMFILTER
