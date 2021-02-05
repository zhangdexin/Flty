#ifndef __LMOUSEEVENT_H__
#define __LMOUSEEVENT_H__

#include "Defines.hpp"
#include "LEvent.hpp"
#include "include/core/SkPoint.h"

class LMouseEvent : public LEvent
{
public:
    enum MouseEventType : unsigned char {
        MouseLeftPressEvent,
        MouseLeftReleaseEvent,
        MouseRightPressEvent,
        MouseRightReleaseEvent,
        MouseLeftDbEvent,
        MouseMoveEvent
    };

    LMouseEvent();
    LMouseEvent(int x, int y);

    void setPostion(int x, int y);
    void setMouseEventType(MouseEventType tp); 
    void setMouseEventType(skui::InputState tp);

    SkIPoint pos() const;
    MouseEventType mouseEventType() const;

private:
    int            m_X;
    int            m_Y;
    MouseEventType m_MouseEventType;
};

#endif // __LMOUSEEVENT_H__
