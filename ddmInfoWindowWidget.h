#ifndef DDMINFOWINDOWWIDGET_H
#define DDMINFOWINDOWWIDGET_H

#include <QString>
#include <QWidget>

namespace Ui {
class ddmInfoWindowWidget;
}

class ddmInfoWindowWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ddmInfoWindowWidget(QWidget *parent = 0);
    ~ddmInfoWindowWidget();

    void writeDebug( const QString& text );

    void writeInfo( const QString& text );

    void writeWarning( const QString& text );

    void writeError( const QString& text );

private slots:
    void slotClearText();

    void ShowContextMenu( const QPoint& point );

private:
    QString getCurrentTime();

    QString getCurrentPathToIcon();

    void insertImage( const QString& fullFileName );

private:
    Ui::ddmInfoWindowWidget *ui;
};

#endif // DDMINFOWINDOWWIDGET_H
