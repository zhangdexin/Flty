#include "LWidget.h"

#include <utility>

#define SET_WIDGET_STYLE_ONE(type, setFunction, addNotifySetOfWnd) do { \
    lstyleTask task = [type](LStyleSheet& style)->void { \
        style.setFunction(type); \
    }; \
    m_StyledChangedQueue->write(task); \
    m_Style.setFunction(type);        \
    if (m_AttachWnd) { \
        m_AttachWnd->addNotifySetOfWnd(shared_from_this()); \
    } \
} while(0)

#define SET_WIDGET_STYLE_TWO(type1, type2, setFunction, addNotifySetOfWnd) do { \
    lstyleTask task = [type1, type2](LStyleSheet& style)->void { \
        style.setFunction(type1, type2); \
    }; \
    m_StyledChangedQueue->write(task); \
    m_Style.setFunction(type1, type2);        \
    if (m_AttachWnd) { \
        m_AttachWnd->addNotifySetOfWnd(shared_from_this()); \
    } \
} while(0)

static long long s_IdGetter = 0;

LWidget::LWidget() :
    m_WidgetId {s_IdGetter++},
    m_StyledChangedQueue{std::make_shared<nvwa::fc_queue<lstyleTask>>(200)}
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
}