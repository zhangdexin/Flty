#include "LWidget.h"

#include <utility>

static long long s_IdGetter = 0;

LWidget::LWidget(const LWidgetSPtr& widget) :
    m_WidgetId {s_IdGetter++}
{
    lApp->postTaskToMainThread([this, widget]() {
        widget->addChildWidget(shared_from_this());
        init();
    });
}

LWidget::LWidget(const LWindowSPtr& window):
    m_WidgetId{ s_IdGetter++ }
{
    lApp->postTaskToMainThread([this, window]() {
        window->addRootChild(shared_from_this());
        init();
    });
}

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

    widget->m_RightSibling = nullptr;
    widget->setAttachWnd(m_AttachWnd);
    widget->setLayerIndex(m_LayerIndex);
    m_ChildWidgets.push_back(widget);
}

void LWidget::setBackgroundColor(const SkColor& color)
{
    m_Style.setBackgroundColor(color);
    m_AttachWnd->addGraphicSet(shared_from_this());
}

void LWidget::setSize(const SkSize& size)
{
    m_Style.setSize(size);
    m_AttachWnd->addLayoutSet(shared_from_this());
}

void LWidget::setAttachWnd(const LWindowSPtr& window)
{
    m_AttachWnd = window;
    for (auto item : m_ChildWidgets) {
        item->setAttachWnd(window);
    }
}

void LWidget::setLayerIndex(unsigned index)
{
    m_LayerIndex = index;
}
