#include "LWidget.h"

#include <utility>

LWidget::LWidget(const LWidgetSPtr& widget)
{
    lApp->addPreExecQueue([this, widget]() {
        widget->addChildWidget(shared_from_this());
        init();
    });
}

LWidget::LWidget(const LWindowSPtr& window)
{
    lApp->addPreExecQueue([this, window]() {
        window->addRootChild(shared_from_this());
        init();
    });
}

void LWidget::init()
{
    m_Style = std::make_unique<LStyleSheet>();
}

void LWidget::addChildWidget(const LWidgetSPtr& widget)
{
    int size = m_ChildWidget.size();
    if (size > 0) {
        widget->m_LeftSibling = m_ChildWidget[size];
        m_ChildWidget[size]->m_RightSibling = widget;
    }
    else {
        widget->m_LeftSibling = nullptr;
    }

    widget->m_RightSibling = nullptr;
    widget->setAttachWnd(m_AttachWnd);
    m_ChildWidget.push_back(widget);
}

void LWidget::setBackgroundColor(const SkColor& color)
{
    m_Style->setBackgroundColor(color);
}

LWindowSPtr LWidget::attachWnd() const
{
    return m_AttachWnd;
}

void LWidget::setAttachWnd(const LWindowSPtr& window)
{
    m_AttachWnd = window;
    for (auto item : m_ChildWidget) {
        item->setAttachWnd(window);
    }
}
