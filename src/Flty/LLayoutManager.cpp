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
        rootNode->m_Style.updateBoundingRect();
    }

    auto& vct = rootNode->m_Children;
    if (vct.size() > 0) {
        
        doLayout(rootNode, vct[0]);
    }
}

void LLayoutManager::doLayout(lshared_ptr<LRenderNode>& parent, lshared_ptr<LRenderNode>& node)
{
    auto& parentStyle = parent->m_Style;
    const auto&& boxType = parentStyle.boxType();

    SkRect&& rt = parentStyle.boundingRect();
    SkPoint offset = SkPoint::Make(rt.x(), rt.y());
    int size = parent->m_Children.size();

    lshared_ptr<LRenderNode> child = node;
    int parentWidth = parentStyle.width();
    int parentHeight = parentStyle.height();
    int childWidth = parentWidth / size;
    int childHeight = parentHeight / size;
    int dxy = 0;
    do {
        auto& style = child->m_Style;
        if (boxType == LBoxType::Horizontal) {
            style.setPos(SkPoint::Make(dxy * childWidth, 0));
            style.setSize(SkSize::Make(childWidth, parentHeight));
        }
        else if (boxType == LBoxType::Vertical) {
            style.setPos(SkPoint::Make(0, dxy * childHeight));
            style.setSize(SkSize::Make(parentWidth, childHeight));
        }

        style.updateBoundingRectByOffset(offset);
        child->setLayoutChanged(false);
        ++dxy;

        // update first children
        auto& vct = child->m_Children;
        if (vct.size() > 0) {
            doLayout(child, vct[0]);
        }

    } while ((child = child->m_RightSibling) != nullptr);
}
