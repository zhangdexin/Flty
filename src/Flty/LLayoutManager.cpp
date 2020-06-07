#include "LLayoutManager.h"

#include "LLayerContext.h"
#include "LRenderNode.hpp"
#include "LStyleSheet.h"
#include "LWidget.h"

LLayoutManager::LLayoutManager(const lshared_ptr<LLayerContext>& layer) :
    m_LayerContext{ layer }
{}

void LLayoutManager::needLayout(const lwidget_sptr& widget)
{
    auto nodePtr = m_LayerContext->node(widget->m_WidgetId);
    if (!nodePtr) {
        return;
    }

    auto &&node = nodePtr->get();
    auto &style = node->m_Style;

    node->setLayoutChanged(true)
        .setGraphicChangedByLayout(true);

    auto &queue = widget->m_StyledChangedQueue;
    while (!queue->empty()) {
        lstyleTask task;
        queue->read(task);
        task(style);
    }
}

void LLayoutManager::layout()
{
    auto& rootNode = m_LayerContext->m_RootNodePtr;
    if (rootNode->m_LayoutChanged) {
        rootNode->m_Style.updateRect();
    }

    auto& vct = rootNode->m_Children;
    if (vct.size() > 0) {
        doLayout(rootNode, vct[0]);
    }
}

void LLayoutManager::doLayout(const lshared_ptr<LRenderNode>& parentNode,
                              const lshared_ptr<LRenderNode>& node)
{
    node->m_Style.updateRectBy(parentNode->m_Style.pos());
    node->setLayoutChanged(false);

    // update m_RightSibling
    // TODO

    auto& vct = node->m_Children;
    if (vct.size() > 0) {
        doLayout(node, vct[0]);
    }
}
