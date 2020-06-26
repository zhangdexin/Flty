#include "LWidget.h"

#include <utility>

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
        m_AttachWnd->onChildWidgetAdd(widget);
    }
}

void LWidget::setBackgroundColor(const SkColor& color)
{
    lstyleTask task = [color](LStyleSheet& style)->void {
        style.setBackgroundColor(color);
    };

    m_StyledChangedQueue->write(task);

    m_Style.setBackgroundColor(color);
    if (m_AttachWnd) {
        m_AttachWnd->addGraphicSet(shared_from_this());
    }
}

void LWidget::setSize(const SkSize& size)
{
    lstyleTask task = [size](LStyleSheet& style) {
        style.setSize(size);
    };
    m_StyledChangedQueue->write(task);

    m_Style.setSize(size);
    if (m_AttachWnd) {
        m_AttachWnd->addLayoutSet(shared_from_this());
    }
}

void LWidget::setPosition(const SkPoint &pt)
{
    lstyleTask task = [pt](LStyleSheet& style) {
        style.setPos(pt);
    };
    m_StyledChangedQueue->write(task);

    m_Style.setPos(pt);
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

void LWidget::setAttachWnd(const lwindow_sptr& window)
{
    m_AttachWnd = window;
    for (auto& item : m_ChildWidgets) {
        item->setAttachWnd(window);
    }
}