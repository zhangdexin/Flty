#ifndef ___LWIDGET_H__
#define ___LWIDGET_H__

#include "include/core/SkColor.h"
#include "include/core/SkSize.h"
#include "WidgetType.hpp"
#include "LStyleSheet.h"
#include "Defines.hpp"
#include "LWindow.h"

class LWidget : public std::enable_shared_from_this<LWidget>
{
public:
    LWidget(const LWidgetSPtr& widget);
    LWidget(const LWindowSPtr& window);

    void init();
    virtual void addChildWidget(const LWidgetSPtr& widget);
    virtual lvct_shared_ptr<LWidget> children() { return m_ChildWidgets; }

    virtual void setBackgroundColor(const SkColor& color);
    virtual void setSize(const SkSize& size);

    virtual unsigned layerIndex() const { return *m_LayerIndexPtr.get(); }
    virtual void setLayerIndex(const lshared_ptr<unsigned>& index);

    virtual lstring className() const {
        return u8"lwidget";
    }

    virtual WidgetType type() const{
        return WidgetType::Widget;
    }

    LWindowSPtr attachWnd() const { return m_AttachWnd; }
    void setAttachWnd(const LWindowSPtr& window);

public:
    LStyleSheet              m_Style;
    const long long          m_WidgetId;

protected:
    LWidgetSPtr              m_RightSibling;
    LWidgetSPtr              m_LeftSibling;
    LWindowSPtr              m_AttachWnd;
    lvct_shared_ptr<LWidget> m_ChildWidgets;
    lshared_ptr<unsigned>    m_LayerIndexPtr;
};

#endif // ___LWIDGET_H__
