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

private:
     void installEvents();

private slots:

     void updateVisualize();

     void slotVerifyData( const QString& text );

private:
    Ui::ddmFrictionCountyFilterWidget *ui;
    ddmFrictionCountyFilter* m_filter;
};

#endif // DDMFRICTIONCOUNTYFILTERWIDGET_H
