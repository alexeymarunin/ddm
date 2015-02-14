#ifndef DDMEMPTYFILTERWIDGET_H
#define DDMEMPTYFILTERWIDGET_H

#include <QWidget>

class ddmEmptyFilter;
class ddmCounty;
class ddmState;

class QLineEdit;

namespace Ui {
class ddmEmptyFilterWidget;
}

class ddmEmptyFilterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ddmEmptyFilterWidget( ddmEmptyFilter* filter,  QWidget *parent = 0);
    ~ddmEmptyFilterWidget();

private slots:
    // установить текуший штат для модели
    void slotSetCurrentState( const QString& stateName );

    // установить текушее графство для модели
    void slotSetCurrentCounty( const QString& countyName );

    // обновить штат
    void changedState( ddmState* state );

    // обновить графство на карте
    void changedCounty( ddmCounty* county );

private:
    void installEvents();
    void updateCountiesList();

private:
    Ui::ddmEmptyFilterWidget *ui;
    ddmEmptyFilter* m_filter;
    QLineEdit*  m_leState;
    QLineEdit*  m_leCounty;
};

#endif // DDMEMPTYFILTERWIDGET_H
