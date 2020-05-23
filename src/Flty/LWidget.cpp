#include "LWidget.h"

#include <utility>

static long long s_IdGetter = 0;

LWidget::LWidget() :
    m_WidgetId {s_IdGetter++}
{}

void LWidget::init()
{
}

void LWidget::addChildWidget(const LWidgetSPtr& widget)
{
    int size = m_ChildWidgets.size();
    if (size > 0) {
        widget->m_LeftSibling = m_ChildWidgets[size];
        m_ChildWidgets[size]->m_RightSibling = widget;
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
        m_AttachWnd->onChildWidgetAdd(widget, *m_LayerIndexPtr);
    }
}

void LWidget::setBackgroundColor(const SkColor& color)
{
    m_Style.setBackgroundColor(color);
    if (m_AttachWnd) {
        m_AttachWnd->addGraphicSet(shared_from_this());
    }
}

void LWidget::setSize(const SkSize& size)
{
    m_Style.setSize(size);
    if (m_AttachWnd) {
        m_AttachWnd->addLayoutSet(shared_from_this());
    }
}

void LWidget::setLayerIndex(const lshared_ptr<unsigned>& index)
{
    m_LayerIndexPtr = index;
    for (auto& item : m_ChildWidgets) {
        item->setLayerIndex(index);
    }
}

void LWidget::setAttachWnd(const LWindowSPtr& window)
{
    m_AttachWnd = window;
    for (auto& item : m_ChildWidgets) {
        item->setAttachWnd(window);
    }
}