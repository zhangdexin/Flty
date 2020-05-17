#include "LLayoutManager.h"

#include "LLayerContext.h"
#include "LRenderNode.hpp"
#include "LStyleSheet.h"
#include "LWidget.h"

LLayoutManager::LLayoutManager(const lshared_ptr<LLayerContext>& layer) :
    m_LayerContext{ layer }
{}

void LLayoutManager::needLayout(const LWidgetSPtr& widget)
{
    lshared_ptr<LRenderNode>&& nodePtr = m_LayerContext->tryEmplaceNodeMap(widget);
    nodePtr->setStyle(widget->m_Style)
        .setWidget(widget)
        .setLayoutChanged(true)
        .setGraphicChangedByLayout(true);
}

void LLayoutManager::layout()
{
    auto& rootNode = m_LayerContext->m_RootNodePtr;
    if (rootNode->m_LayoutChanged) {
        rootNode->m_Style.updateRect();
    }

    auto&& vct = rootNode->m_Widget->children();
    if (vct.size() > 0) {
        doLayout(rootNode->m_Widget, vct[0]);
    }
}

void LLayoutManager::doLayout(const LWidgetSPtr& parentWidget, const LWidgetSPtr& widget)
{
    auto parentId = parentWidget->m_WidgetId;
    auto id = widget->m_WidgetId;

    auto&& parentNodeOpt = m_LayerContext->node(parentId);
    auto&& nodeOpt = m_LayerContext->node(id);

    if (!parentNodeOpt || !nodeOpt) {
        return;
    }

    auto&& parentNode = parentNodeOpt->get();
    auto&& node = nodeOpt->get();
    //if (node->m_LayoutChanged) {
    //    return;
    //}

    node->m_Style.updateRectBy(parentNode->m_Style.pos());
    node->setLayoutChanged(false)
        .setGraphicChangedByLayout(true);

    // update m_RightSibling
    // TODO

    auto&& vct = widget->children();
    if (vct.size() > 0) {
        doLayout(widget, vct[0]);
    }
}
