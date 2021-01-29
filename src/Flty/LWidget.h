#ifndef ___LWIDGET_H__
#define ___LWIDGET_H__

#include "include/core/SkColor.h"
#include "include/core/SkSize.h"
#include "WidgetType.hpp"
#include "LStyleSheet.h"
#include "Defines.hpp"
#include "LWindow.h"
#include "base/fc_queue.h"

#define SET_WIDGET_STYLE_ONE(type, setFunction, addNotifySetOfWnd) do { \
    lstyleTask task = [type](LStyleSheet& style)->void { \
        style.setFunction(type); \
    }; \
    m_StyledChangedQueue->write(task); \
    m_Style.setFunction(type);         \
    if (m_AttachWnd) { \
        m_AttachWnd->addNotifySetOfWnd(shared_from_this()); \
    } \
} while(0)

#define SET_WIDGET_STYLE_TWO(type1, type2, setFunction, addNotifySetOfWnd) do { \
    lstyleTask task = [type1, type2](LStyleSheet& style)->void { \
        style.setFunction(type1, type2); \
    }; \
    m_StyledChangedQueue->write(task); \
    m_Style.setFunction(type1, type2);        \
    if (m_AttachWnd) { \
        m_AttachWnd->addNotifySetOfWnd(shared_from_this()); \
    } \
} while(0)

class LWidget : public std::enable_shared_from_this<LWidget>
{
public:
    LWidget();

    void init();
    virtual void addChildWidget(const lwidget_sptr& widget);
    virtual lvct_shared_ptr<LWidget> children() { return m_ChildWidgets; }

    void setBackgroundColor(const SkColor& color);
    void setFixedSize(const SkISize& size); // fixed
    void setFixedWidth(int width); // fixed
    void setFixedHeight(int height); // fixed
    void setPosition(const SkIPoint& pt); // relative parent
    void setPositon(int x, int y) {
        setPosition(SkIPoint::Make(x, y));
    }
    void setBox(LBoxType type);
    void setMinSize(const SkISize& size);
    void setMaxSize(const SkISize& size);
    void setSizePolicy(LSizePolicy widthPolicy, LSizePolicy hegihtPolicy);
    void setBorder(int width, LBorderStyle s, const SkColor& c);
    void setBorderRadius(int radius);
    void setMargin(const std::initializer_list<int>& lt);
    void setPadding(const std::initializer_list<int>& lt);

    virtual unsigned layerIndex() const { 
        if (m_LayerIndexPtr) {
            return *m_LayerIndexPtr.get();
        }
        return 0;
    }
    virtual void setLayerIndex(const lshared_ptr<unsigned>& index);

    virtual lstring className() const {
        return u8"lwidget";
    }

    virtual WidgetType type() const{
        return WidgetType::Widget;
    }

    lwindow_sptr attachWnd() const { return m_AttachWnd; }
    void setAttachWnd(const lwindow_sptr& window);

    lwidget_sptr parent() const { return m_ParentPtr; }
    lvct_shared_ptr<LWidget> children() const { return m_ChildWidgets; }

public:
    LStyleSheet                 m_Style;
    const int                   m_WidgetId;

    using lstyle_queue_ptr = lshared_ptr<nvwa::fc_queue<lstyleTask>>;
    lstyle_queue_ptr            m_StyledChangedQueue;

protected:
    lwidget_sptr             m_ParentPtr = nullptr;
    lwidget_sptr             m_RightSibling = nullptr;
    lwidget_sptr             m_LeftSibling = nullptr;
    lwindow_sptr             m_AttachWnd = nullptr;
    lvct_shared_ptr<LWidget> m_ChildWidgets;

    lshared_ptr<unsigned>    m_LayerIndexPtr = nullptr;
};

#endif // ___LWIDGET_H__
