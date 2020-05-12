#ifndef __LLABEL_H__
#define __LLABEL_H__

#include "LWidget.h"
#include "LWindow.h"

class LLabel: public LWidget
{
public:
    LLabel(const LWidgetSPtr& widget);
    LLabel(const LWindowSPtr& window);

    virtual lstring className() const {
        return u8"llabel";
    }

    virtual WidgetType type() const {
        return WidgetType::Label;
    }

};

#endif // __LLABEL_H__