#ifndef ___LWIDGET_H__
#define ___LWIDGET_H__

#include "include/core/SkColor.h"
#include "include/core/SkSize.h"
#include "WidgetType.hpp"
#include "LStyleSheet.h"
#include "Defines.hpp"
#include "LWindow.h"
#include "base/fc_queue.h"

class LWidget : public std::enable_shared_from_this<LWidget>
{
public:
    LWidget();

    void init();
    virtual void addChildWidget(const lwidget_sptr& widget);
    virtual lvct_shared_ptr<LWidget> children() { return m_ChildWidgets; }

    // TODO: virtual
    virtual void setBackgroundColor(const SkColor& color);
    virtual void setFixedSize(const SkISize& size); // fixed
    virtual void setFixedWidth(int width); // fixed
    virtual void setFixedHeight(int height); // fixed
    virtual void setPosition(const SkIPoint& pt); // relative parent
    virtual void setPositon(int x, int y) {
        setPosition(SkIPoint::Make(x, y));
    }
    virtual void setBox(LBoxType type);
    virtual void setMinSize(const SkISize& size);
    virtual void setMaxSize(const SkISize& size);
    virtual void setSizePolicy(LSizePolicy widthPolicy, LSizePolicy hegihtPolicy);

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

public:
    LStyleSheet                 m_Style;
    const long long             m_WidgetId;

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
