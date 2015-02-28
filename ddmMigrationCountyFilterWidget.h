#ifndef DDMMIGRATIONCOUNTYFILTERWIDGET_H
#define DDMMIGRATIONCOUNTYFILTERWIDGET_H

#include <QWidget>

class QLineEdit;

namespace Ui {
class ddmMigrationCountyFilterWidget;
}

class ddmMigrationCountyFilter;

class ddmMigrationCountyFilterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ddmMigrationCountyFilterWidget( ddmMigrationCountyFilter* filter, QWidget *parent = 0);
    ~ddmMigrationCountyFilterWidget();


private:
     void installEvents();

private slots:
     void slotEnableMisgrationRange( int state );

private:
    Ui::ddmMigrationCountyFilterWidget *ui;
    ddmMigrationCountyFilter* m_filter;

    QLineEdit *m_leState;
    QLineEdit *m_leCounty;
};

#endif // DDMMIGRATIONCOUNTYFILTERWIDGET_H
