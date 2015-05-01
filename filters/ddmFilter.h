#ifndef DDM_FILTER_H
#define DDM_FILTER_H

#include "models/ddmFilterModel.h"
#include "widgets/ddmFilterWidget.h"

class ddmMapView;
class ddmCenterPoint;

/**
 * Класс ddmFilter является базовым классом для всех остальных фильтров
 * Определяет основные поля, методы, сигналы и слоты
 * Предоставляет доступ к модели данных, виджету и карте
 *
 * Для отображения графств на карте используется список selection, который содержит только
 * те графства, которые должны быть отображены в текущий момент. По умолчанию в этот список
 * входят все графства, для которых свойство visible = true
 *
 * Методы класса:
 *  - setup()
 *    Защищенный метод, служит для формирования внутренних полей фильтра.
 *    По умолчанию проверяет наличие модели и виджета, загружает модель и задает основные сигнал-слот связи.
 *    Должен быть перегружен в наследуемых классах, в перегруженном методе должны создаваться модель и виджет,
 *    а затем обязательно должен вызываться метод из базового класса (чтобы корректно завершить создание).
 *  - apply()
 *    Открытый метод, запускающий выполнение фильтра (синхронизацию данных, выборку и отображение на карте)
 *    По умолчанию заносит данные из виджета в модель и обновляет текущее выделение. Если оно было изменено, то обновление
 *    карты произойдет автоматически.
 *    Может быть перегружен в наследуемых классах.
 *  - activate()
 *    Открытый метод, вызывает активацию (отображение) фильтра
 *  - deactivate()
 *    Открытый метод, вызывает деактивацию (скрытие) фильтра
 *  - updateData()
 *    Защищенный метод, служит для синхронизации данных между моделью и виджетом.
 *    По умолчанию ничего не делает. Может быть перегружен в наследуемом классах.
 *  - updateSelection()
 *    Защищенный метод, служит для формирования списка selection.
 *    По умолчанию ничего не делает. Может быть перегружен в наследуемых классах.
 *
 * Сигналы:
 *  - modelChanged() получен сигнал changed() от модели (что-то изменилось в данных)
 *  - widgetChanged() получен сигнал changed() от виджета (что-то изменилось на виджете)
 *  - selectionUpdated() изменился список отображаемых графств
 *
 * Слоты:
 *  - slotModelChanged() вызывается при получении сигнала changed() от модели
 *  - slotWidgetChanged() вызывается при получении сигнала changed() от виджета
 *  - slotMapLoaded() вызывается после полной загрузки карты в JavaScript
 *  - slotJavaScriptWindowObjectCleared() вызывается по окончании загрузки страницы
 *
 * @author  Марунин А.В.
 * @since   2.0
 */
class ddmFilter : public ddmObject
{
    Q_OBJECT

public:

    ddmFilter( QObject* parent = NULL );

    // Список выбранных (отображаемых) графств фильтра
    Q_PROPERTY( QVariantList selection READ selection )

    virtual void resetSelection();
    virtual QVariantList selection() const;

    virtual void apply();
    void activate( bool autoApplay = false );
    void deactivate();

    ddmFilterModel*  model() const;
    ddmFilterWidget* widget() const;
    ddmMapView*      mapView() const;

    void setMapCenter( const QVariantMap& center );
    void setMapCenter( double x, double y );

    bool valid() const;
    bool isMapLoaded() const;

    void drawArrow( ddmCenterPoint* begPoint, ddmCenterPoint* endPoint, double width );

    virtual ~ddmFilter();


Q_SIGNALS:

    void modelChanged();
    void widgetChanged();
    void selectionUpdated();

protected Q_SLOTS:

    virtual void updateVisualzation();
    virtual void slotModelChanged();
    virtual void slotWidgetChanged();

    virtual void slotJavaScriptWindowObjectCleared();
    virtual void slotMapLoaded();

protected:

    ddmFilterModel*     m_model;
    ddmFilterWidget*    m_widget;
    ddmMapView*         m_mapView;

    template<typename T> T* model_cast() const;
    template<typename T> T* widget_cast() const;

    virtual void setup();

    virtual void updateData( bool fromWidget = true );
    virtual void updateSelection();

    virtual void loadSettings();
    virtual void saveSettings();
};

template<typename T>
T* ddmFilter::model_cast() const
{
    return qobject_cast<T*>( this->model() );
}

template<typename T>
T* ddmFilter::widget_cast() const
{
    return qobject_cast<T*>( this->widget() );
}

#endif // DDM_FILTER_H

