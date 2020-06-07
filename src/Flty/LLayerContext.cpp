#include "LLayerContext.h"

#include "SkSurface.h"
#include "LLayerContext.h"

LLayerContext::LLayerContext(const lwidget_sptr& widget, unsigned index) :
    m_LayerIndexPtr{ std::make_shared<unsigned>(index) },
    m_Surface{ SkSurface::MakeRasterN32Premul(800, 600) }
{
    addRootNode(widget);
}

LLayerContext::LLayerContext(const lwidget_sptr& widget)
{
    addRootNode(widget);
}

void LLayerContext::addRootNode(const lwidget_sptr& widget)
{
    m_RenderNodeMap.emplace(
        widget->m_WidgetId,
        std::make_shared<LRenderNode>(widget->m_WidgetId));
    m_RootNodePtr = m_RenderNodeMap[widget->m_WidgetId];

    for (auto &iter : widget->children()) {
        addChildNode(iter);
    }
}

void LLayerContext::addChildNode(const lwidget_sptr& widget)
{
    auto parent = widget->parent();
    if (parent == nullptr) {
        return;
    }
    auto parentId = parent->m_WidgetId;
    auto &&iter = m_RenderNodeMap.find(parentId);
    if (iter == m_RenderNodeMap.end()) {
        return;
    }

    auto curId = widget->m_WidgetId;
    if (m_RenderNodeMap.find(curId) != m_RenderNodeMap.end()) {
        return;
    }

    auto &parentNode = iter->second;
    auto &children = parentNode->m_Children;
    auto childSize = children.size();

    lshared_ptr<LRenderNode> node = std::make_shared<LRenderNode>(curId);
    node->m_Parent = parentNode;
    if (childSize > 0) {
        children[childSize - 1]->m_RightSibling = node;
        node->m_LeftSibling = children[childSize - 1];
    }
    children.push_back(node);
    m_RenderNodeMap[curId] = node;

    for (auto& item : widget->children()) {
        addChildNode(item);
    }
}

loptional<lshared_ptr<LRenderNode>> LLayerContext::node(long long id)
{
    if (m_RenderNodeMap.find(id) != m_RenderNodeMap.end()) {
        return m_RenderNodeMap[id];
    }
    return std::nullopt;
}

void LLayerContext::appendLayerContextNode(const lshared_ptr<LLayerContext>& contextPtr, long long parentId)
{
    auto iter = m_RenderNodeMap.find(parentId);
    if (iter != m_RenderNodeMap.end()) {
        auto& rootNode = contextPtr->m_RootNodePtr;
        auto &children = iter->second->m_Children;
        auto childSize = children.size();
        if (childSize > 0) {
            children[childSize - 1]->m_RightSibling = rootNode;
            rootNode->m_LeftSibling = children[childSize - 1];
        }

        children.push_back(std::move(rootNode));
        m_RenderNodeMap.insert(contextPtr->m_RenderNodeMap.begin(), contextPtr->m_RenderNodeMap.end());
    }
}

void LLayerContext::graphic()
{
    auto canvas = m_Surface->getCanvas();
    canvas->clear(SK_ColorWHITE);

    auto& style = m_RootNodePtr->m_Style;

    SkPaint paint;
    paint.setColor(style.backgroundColor());
    canvas->drawRect(style.rect(), paint);

    if (m_RootNodePtr->m_Children.size() > 0) {
        doChildGraphic(canvas, m_RootNodePtr, m_RootNodePtr->m_Children[0]);
    }
}

void LLayerContext::doChildGraphic(SkCanvas* canvas,
                                   const lshared_ptr<LRenderNode>& parentNode,
                                   const lshared_ptr<LRenderNode>& node)
{
    const auto& style = node->m_Style;

    SkPaint paint;
    paint.setColor(style.backgroundColor());
    canvas->drawRect(style.rect(), paint);

    if (node->m_Children.size() > 0) {
        doChildGraphic(canvas, node, node->m_Children[0]);
    }
}
