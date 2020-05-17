#include "LLayerContext.h"

#include "SkSurface.h"
#include "LLayerContext.h"

LLayerContext::LLayerContext(const LWidgetSPtr& widget, unsigned index) :
    m_LayerIndexPtr{ std::make_shared<unsigned>(index) },
    m_Surface{ SkSurface::MakeRasterN32Premul(800, 600) } {

    m_RenderNodeMap.emplace(widget->m_WidgetId, std::make_shared<LRenderNode>(widget, true));
    m_RootNodePtr = m_RenderNodeMap[widget->m_WidgetId];
}

lshared_ptr<LRenderNode> LLayerContext::tryEmplaceNodeMap(const LWidgetSPtr& widget)
{
    m_RenderNodeMap.try_emplace(widget->m_WidgetId, std::make_shared<LRenderNode>(widget, true));
    return m_RenderNodeMap[widget->m_WidgetId];
}

loptional<lshared_ptr<LRenderNode>> LLayerContext::node(long long id)
{
    if (m_RenderNodeMap.find(id) != m_RenderNodeMap.end()) {
        return m_RenderNodeMap[id];
    }
    return std::nullopt;
}

void LLayerContext::graphic()
{
    auto canvas = m_Surface->getCanvas();
    canvas->clear(SK_ColorWHITE);

    auto& style = m_RootNodePtr->m_Style;

    SkPaint paint;
    paint.setColor(style.backgroundColor());
    canvas->drawRect(style.rect(), paint);

    if (m_RootNodePtr->m_Widget->children().size() > 0) {
        doChildGraphic(canvas, m_RootNodePtr, m_RootNodePtr->m_Widget->children()[0]);
    }
}

void LLayerContext::doChildGraphic(SkCanvas* canvas,
        const lshared_ptr<LRenderNode>& parentNode,
        const LWidgetSPtr& firstChildWidget)
{
    auto id = firstChildWidget->m_WidgetId;
    const auto& node = m_RenderNodeMap[id];
    const auto& style = node->m_Style;

    SkPaint paint;
    paint.setColor(style.backgroundColor());
    canvas->drawRect(style.rect(), paint);

    if (firstChildWidget->children().size() > 0) {
        doChildGraphic(canvas, node, firstChildWidget->children()[0]);
    }
}
