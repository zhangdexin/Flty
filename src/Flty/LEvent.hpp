#ifndef __LEVENT_HPP__
#define __LEVENT_HPP__

class LEvent
{
public:
    virtual ~LEvent() {}

    enum EventType {
        MouseEvent,
        KeyEvent,
    };

    EventType type() const {
        return m_Type;
    }

    void setType(LEvent::EventType tp) {
        m_Type = tp;
    }

protected:
    EventType m_Type;
};

#endif // __LEVENT_HPP__