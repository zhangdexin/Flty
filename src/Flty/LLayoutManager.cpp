#include "LLayoutManager.h"

#include "LStyleSheet.h"
#include "LWidget.h"

LLayoutManager::LLayoutManager(const LWidgetSPtr& widget)
{
    widget->m_Style.updateRect();

    WidgetRenderNode node{ widget, true };
    node.setStyle(widget->m_Style).setLayoutChanged(false).setGraphicChanged(true);
    m_RenderNodeMap.emplace(widget->m_WidgetId, std::move(node));
    m_RootNode = &m_RenderNodeMap[widget->m_WidgetId];
}

void LLayoutManager::needLayout(const LWidgetSPtr& widget)
{
    auto& id = widget->m_WidgetId;
    auto iter = m_RenderNodeMap.find(id);
    if (iter != m_RenderNodeMap.end() &&
            iter->second.m_LayoutChanged == false) {
        return;
    }

    WidgetRenderNode* node = &m_RenderNodeMap[id];
    node->setStyle(widget->m_Style).setWidget(widget).setLayoutChanged(true).setGraphicChanged(true);
}

void LLayoutManager::layout()
{
    if (m_RootNode->m_LayoutChanged) {
        m_RootNode->m_Style.updateRect();
    }

    auto&& vct = m_RootNode->m_Widget->children();
    if (vct.size() > 0) {
        doLayout(m_RootNode->m_Widget, vct[0]);
    }
}

void LLayoutManager::doLayout(const LWidgetSPtr& parentWidget, const LWidgetSPtr& widget)
{
    auto parentId = parentWidget->m_WidgetId;
    auto id = widget->m_WidgetId;

    auto& parentNode = m_RenderNodeMap[parentId];
    auto& node = m_RenderNodeMap[id];

    if (!node.m_LayoutChanged) {
        return;
    }

    node.m_Style.updateRectBy(parentNode.m_Style.pos());

    // update m_RightSibling
    // TODO

    auto&& vct = widget->children();
    if (vct.size() > 0) {
        doLayout(widget, vct[0]);
    }
}

bool LLayoutManager::compareLayoutAndCopy(LStyleSheet& style)
{
    return false;
}
