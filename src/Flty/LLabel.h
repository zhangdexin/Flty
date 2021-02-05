#ifndef __LLABEL_H__
#define __LLABEL_H__

#include "LWidget.h"
#include "LWindow.h"

class LLabel: public LWidget
{
public:
    LLabel();

    void setText(const lstring& text);
    void setTextColor(const SkColor& color);

    virtual WidgetType type() const {
        return WidgetType::Label;
    }

    void addClickAction(const std::function<void(void)>& action) {
        m_ClickActions.push_back(action);
    }

    virtual bool mouseReleaseEvent(const lshared_ptr<LMouseEvent>& ev); // left

protected:
    lvct<std::function<void(void)>> m_ClickActions;
};

#endif // __LLABEL_H__