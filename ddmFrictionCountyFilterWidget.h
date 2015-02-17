#ifndef DDMFRICTIONCOUNTYFILTERWIDGET_H
#define DDMFRICTIONCOUNTYFILTERWIDGET_H

#include <QWidget>

class ddmFrictionCountyFilter;

namespace Ui {
class ddmFrictionCountyFilterWidget;
}

class ddmFrictionCountyFilterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ddmFrictionCountyFilterWidget( ddmFrictionCountyFilter* filter, QWidget *parent = 0);
    ~ddmFrictionCountyFilterWidget();

private slots:
    void slotIntervalMode( bool state );
    void slotValueMode( bool state );


private:
     void installEvents();

private:
    Ui::ddmFrictionCountyFilterWidget *ui;
    ddmFrictionCountyFilter* m_filter;
};

#endif // DDMFRICTIONCOUNTYFILTERWIDGET_H
