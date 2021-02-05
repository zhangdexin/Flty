#ifndef __LBUTTON_H__
#define __LBUTTON_H__

#include "LLabel.h"

class LButton : public LLabel
{
public:
    virtual WidgetType type() const {
        return WidgetType::Button;
    }
};

#endif // __LBUTTON_H__
