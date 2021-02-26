#include "LWidget.h"

#include <utility>

#include "LEvent.hpp"
#include "LMouseEvent.h"

static int s_IdGetter = 0;

LWidget::LWidget() :
    m_WidgetId {s_IdGetter++},
    m_StyledChangedQueue{std::make_shared<nvwa::fc_queue<lstyleTask>>(200)}
{}

LWidget::~LWidget()
{}

void LWidget::init()
{}

void LWidget::addChildWidget(const lwidget_sptr& widget)
{
    int size = m_ChildWidgets.size();
    if (size > 0) {
        widget->m_LeftSibling = m_ChildWidgets[size - 1];
        m_ChildWidgets[size - 1]->m_RightSibling = widget;
    }
    else {
        widget->m_LeftSibling = nullptr;
    }

    widget->m_ParentPtr = shared_from_this();
    widget->m_RightSibling = nullptr;
    widget->setAttachWnd(m_AttachWnd);
    widget->setLayerIndex(m_LayerIndexPtr);
    m_ChildWidgets.push_back(widget);

    if (m_AttachWnd) {
        m_AttachWnd->onChildWidgetAdd(widget);
    }
}

void LWidget::setBackgroundColor(const SkColor& color)
{
    SET_WIDGET_STYLE_ONE(color, setBackgroundColor, addGraphicSet);
}

void LWidget::setFixedSize(const SkISize& size)
{
    SET_WIDGET_STYLE_ONE(size, setFixedSize, addLayoutSet);
}

void LWidget::setFixedWidth(int width)
{
    SET_WIDGET_STYLE_ONE(width, setFixedWidth, addLayoutSet);
}

void LWidget::setFixedHeight(int height)
{
    SET_WIDGET_STYLE_ONE(height, setFixedHeight, addLayoutSet);
}

void LWidget::setPosition(const SkIPoint &pt)
{
    SET_WIDGET_STYLE_ONE(pt, setPos, addLayoutSet);
}

void LWidget::setBox(LBoxType type)
{
    SET_WIDGET_STYLE_ONE(type, setBoxType, addLayoutSet);
}

void LWidget::setMinSize(const SkISize& size)
{
    SET_WIDGET_STYLE_ONE(size, setMinSize, addLayoutSet);
}

void LWidget::setMaxSize(const SkISize& size)
{
    SET_WIDGET_STYLE_ONE(size, setMaxSize, addLayoutSet);
}

void LWidget::setSizePolicy(LSizePolicy widthPolicy, LSizePolicy heightPolicy)
{
    SET_WIDGET_STYLE_TWO(widthPolicy, heightPolicy, setSizePolicy, addLayoutSet);
}

void LWidget::setBorder(int width, LBorderStyle s, const SkColor& c)
{
    LBorder b = { width, s, c };
    SET_WIDGET_STYLE_ONE(b, setBorder, addGraphicSet);
}

void LWidget::setBorderRadius(int radius)
{
    SET_WIDGET_STYLE_ONE(radius, setBorderRadius, addGraphicSet);
}

void LWidget::setMargin(const std::initializer_list<int>& lt)
{
    SET_WIDGET_STYLE_ONE(lt, setMargin, addLayoutSet);
}

void LWidget::setPadding(const std::initializer_list<int>& lt)
{
    SET_WIDGET_STYLE_ONE(lt, setPadding, addLayoutSet);
}

bool LWidget::isExtend() const
{
    return m_IsExtend;
}

void LWidget::setExtend(bool isExtend)
{
    m_IsExtend = isExtend;
    extendSize();
}

void LWidget::setLayerIndex(const lshared_ptr<unsigned>& index)
{
    m_LayerIndexPtr = index;
    for (auto& item : m_ChildWidgets) {
        item->setLayerIndex(index);
    }
}

void LWidget::setAttachWnd(const lwindow_sptr& window)
{
    m_AttachWnd = window;
    for (auto& item : m_ChildWidgets) {
        item->setAttachWnd(window);
    }

    extendSize();
}

bool LWidget::event(const lshared_ptr<LEvent>& ev)
{
    auto doHandle = [ev, this](lshared_ptr<LWidget> child_ptr) ->bool {
        auto evType = ev->type();
        switch (evType) {
        case LEvent::MouseEvent:
        {
            const lshared_ptr<LMouseEvent> mouseEv = std::dynamic_pointer_cast<LMouseEvent>(ev);
            if (!child_ptr) {
                return dispatchMouseEvent(mouseEv);
            }

            if (child_ptr->isContainsPt(mouseEv->pos())) {
                return child_ptr->event(ev);
            }
        }

        case LEvent::KeyEvent:
            break;

        default:
            break;
        }

        return false;
    };

    int size = m_ChildWidgets.size();
    bool isHandled = false;
    for (int i = size - 1; i >= 0 && !isHandled; --i) {
        isHandled = doHandle(m_ChildWidgets.at(i));
    }

    if (size == 0 || !isHandled) {
        return doHandle(nullptr);
    }

    return true;
}

bool LWidget::dispatchMouseEvent(const lshared_ptr<LMouseEvent>& ev)
{
    switch (ev->mouseEventType()) {
    case LMouseEvent::MouseLeftPressEvent:
        return mousePressEvent(ev);

    case LMouseEvent::MouseLeftReleaseEvent:
        return mouseReleaseEvent(ev);

    default:
        return false;
    }
}

void LWidget::extendSize()
{
    if (!m_IsExtend) {
        return;
    }

    if (!m_AttachWnd) {
        return;
    }

    auto wndSize = m_AttachWnd->wndSize();
    if (m_Style.size().equals(wndSize.fWidth, wndSize.fHeight)) {
        return;
    }

    setFixedSize(wndSize);
}

bool LWidget::mousePressEvent(const lshared_ptr<LMouseEvent>& ev)
{
    return false;
}

bool LWidget::mouseReleaseEvent(const lshared_ptr<LMouseEvent>& ev)
{
    return false;
}

bool LWidget::isContainsPt(const SkIPoint& pt) const
{
    auto&& pos = m_Style.pos();
    auto&& size = m_Style.size();
    if (pt.x() >= pos.fX && pt.x() <= pos.fX + size.fWidth &&
            pt.y() >= pos.fY && pt.y() <= pos.fY + size.fHeight) {
        return true;
    }

    return false;
}
