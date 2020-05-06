#ifndef ___LWIDGET_H__
#define ___LWIDGET_H__

#include "include/core/SkColor.h"
#include "LStyleSheet.h"
#include "Defines.hpp"
#include "LWindow.h"

class LWidget : public std::enable_shared_from_this<LWidget>
{
public:
    LWidget(const LWidgetSPtr& widget);
    LWidget(const LWindowSPtr& window);

    void init();
    void addChildWidget(const LWidgetSPtr& widget);
    void setBackgroundColor(const SkColor& color);

    LWindowSPtr attachWnd() const;
    void setAttachWnd(const LWindowSPtr& window);

public:
    lunique_ptr<LStyleSheet> m_Style;

protected:
    LWidgetSPtr              m_RightSibling;
    LWidgetSPtr              m_LeftSibling;
    LWindowSPtr              m_AttachWnd;

private:
    lvct_shared_ptr<LWidget> m_ChildWidget;
};

#endif // ___LWIDGET_H__
