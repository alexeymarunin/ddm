#ifndef DDMTABLEVIEW_H
#define DDMTABLEVIEW_H

#include <QDialog>

#include "ddmTableViewModel.h"


namespace Ui {
class ddmTableView;
}


class ddmTableView : public QDialog
{
    Q_OBJECT

public:
    explicit ddmTableView( QWidget *parent = 0 );
    ~ddmTableView();

protected:
    void resizeEvent( QResizeEvent *event );

private:
    void installEventFilter();

private slots:
    void slotImportData();

    void slotExportData();

    void slotPrint();

signals:
    void dialogDeleted( unsigned int index );

private:
    Ui::ddmTableView *ui;
    ddmTableViewModel *m_model;
};

#endif // DDMTABLEVIEW_H
