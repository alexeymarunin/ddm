#ifndef DDM_FILTER_WIDGET_H
#define DDM_FILTER_WIDGET_H

#include <QVBoxLayout>
#include <QWidget>

class ddmFilter;

/**
 * Класс ddmFilterWidget является базовым классом для всех остальных
 * виджетов, которые используются для задания параметров фильтров
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
class ddmFilterWidget : public QWidget
{
    Q_OBJECT

public:

    ddmFilterWidget( ddmFilter* filter, QWidget* parent = NULL );

    ddmFilter* filter() const;

    virtual ~ddmFilterWidget();

Q_SIGNALS:

    void changed();

public Q_SLOTS:

    void slotApplyFilter();
    void show();
    void hide();

protected:

    ddmFilter*      m_filter;
    QVBoxLayout*    m_widgetLayout;

};

#endif // DDM_FILTER_WIDGET_H
