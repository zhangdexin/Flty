#include "LLayerContext.h"

#include "SkSurface.h"
#include "LLayerContext.h"

LLayerContext::LLayerContext(const lwidget_sptr& widget)
{
    addRootNode(widget);
}

void LLayerContext::initCanvas(int width, int height)
{
    if (m_Surface && m_Surface->width() == width && m_Surface->height() == height) {
        return;
    }

    m_Surface = SkSurface::MakeRasterN32Premul(width, height);
}

void LLayerContext::setLayerIndex(unsigned index)
{
    m_LayerIndexPtr = std::make_shared<unsigned>(index);
}

void LLayerContext::addRootNode(const lwidget_sptr& widget)
{
    m_RenderNodeMap.emplace(
        widget->m_WidgetId,
        std::make_shared<LRenderNode>(widget->m_WidgetId));
    m_RootNodePtr = m_RenderNodeMap[widget->m_WidgetId];

    auto& style = m_RootNodePtr->m_Style;
    auto &queue = widget->m_StyledChangedQueue;
    while (!queue->empty()) {
        lstyleTask task;
        queue->read(task);
        task(style);
    }

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

    // new node and set info
    lshared_ptr<LRenderNode> node = std::make_shared<LRenderNode>(curId);
    auto& style = node->m_Style;
    auto &queue = widget->m_StyledChangedQueue;
    while (!queue->empty()) {
        lstyleTask task;
        queue->read(task);
        task(style);
    }

    // set node relation
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

loptional<lshared_ptr<LRenderNode>> LLayerContext::node(int id)
{
    if (m_RenderNodeMap.find(id) != m_RenderNodeMap.end()) {
        return m_RenderNodeMap[id];
    }
    return std::nullopt;
}

void LLayerContext::appendLayerContextNode(const lshared_ptr<LLayerContext>& contextPtr, int parentId)
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
    if (!m_Surface) {
        return;
    }

    auto canvas = m_Surface->getCanvas();
    canvas->clear(SK_ColorWHITE);

    auto& style = m_RootNodePtr->m_Style;
    paintBody(style, canvas);

    if (m_RootNodePtr->m_Children.size() > 0) {
        doChildGraphic(canvas, m_RootNodePtr->m_Children[0]);
    }
}

void LLayerContext::doChildGraphic(SkCanvas* canvas, const lshared_ptr<LRenderNode>& node)
{
    const auto& style = node->m_Style;
    paintBody(style, canvas);

    auto& siblingNode = node->m_RightSibling;
    if (siblingNode) {
        doChildGraphic(canvas, siblingNode);
    }

    if (node->m_Children.size() > 0) {
        doChildGraphic(canvas, node->m_Children[0]);
    }
}

void LLayerContext::paintBody(const LStyleSheet& style, SkCanvas* canvas)
{
    auto border = style.border();
    if (border.m_BStyle != LBorderStyle::None) {
        SkPaint paint;
        paint.setColor(style.backgroundColor());
        canvas->drawIRect(style.boundingRect().makeInset(border.m_Width, border.m_Width), paint);

        int inset = border.m_Width / 2;
        paintBorder(border, paint);

        SkIRect rt = style.boundingRect().makeInset(inset, inset);
        SkRRect rrt = SkRRect::MakeRectXY(SkRect::Make(rt), (SkScalar)border.m_BorderRadius, (SkScalar)border.m_BorderRadius);
        canvas->drawRRect(rrt, paint);
    }
    else {
        SkPaint paint;
        paint.setColor(style.backgroundColor());
        canvas->drawIRect(style.boundingRect(), paint);
    }
}

void LLayerContext::paintBorder(const LBorder& border, SkPaint& paint)
{
    paint.setAntiAlias(true);
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth((SkScalar)border.m_Width);
    paint.setColor(border.m_BColor);
}

SkIRect LLayerContext::validBoundRect() const
{
    return m_RootNodePtr->m_Style.boundingRect();
}
