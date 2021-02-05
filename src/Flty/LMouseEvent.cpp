#include "LMouseEvent.h"

LMouseEvent::LMouseEvent() :
    LMouseEvent(0, 0)
{}

LMouseEvent::LMouseEvent(int x, int y) :
    m_X(x),
    m_Y(y)
{
    m_Type = LEvent::MouseEvent;
}

void LMouseEvent::setPostion(int x, int y)
{
    m_X = x;
    m_Y = y;
}

void LMouseEvent::setMouseEventType(LMouseEvent::MouseEventType tp)
{
    m_MouseEventType = tp;
}

void LMouseEvent::setMouseEventType(skui::InputState tp)
{
    switch (tp) {
    case skui::InputState::kDown:
        m_MouseEventType = LMouseEvent::MouseLeftPressEvent;
        break;

    case skui::InputState::kUp:
        m_MouseEventType = LMouseEvent::MouseLeftReleaseEvent;
        break;

    case skui::InputState::kMove:
        m_MouseEventType = LMouseEvent::MouseMoveEvent;
        break;

    // TODO:
    default:
        break;
    }
}

SkIPoint LMouseEvent::pos() const
{
    return SkIPoint::Make(m_X, m_Y);
}

LMouseEvent::MouseEventType LMouseEvent::mouseEventType() const
{
    return m_MouseEventType;
}
