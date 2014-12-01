#ifndef DDMMAPSWIDGET_H
#define DDMMAPSWIDGET_H

#include <QWidget>

class QLineEdit;
class ddmObjectModel;

namespace Ui {
class ddmMapsWidget;
}

class ddmMapsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ddmMapsWidget(QWidget *parent = 0 );
    ~ddmMapsWidget();

private slots:
    void slotSetCurentState( const QString& text );

    void slotSetCurentCounty( const QString& text );

    void slotSetCurentState();

    void slotSetCurentCounty();
private:
    Ui::ddmMapsWidget *ui;

    QLineEdit* m_leState;
    QLineEdit* m_leCounty;

    ddmObjectModel* m_model;
};

#endif // DDMMAPSWIDGET_H
