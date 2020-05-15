#ifndef __LLAYERCONTEXT_HPP__
#define __LLAYERCONTEXT_HPP__

#include "Defines.hpp"
#include "LWidget.h"
#include "LRenderNode.hpp"

class LLayerContext
{
public:
    LLayerContext(const LWidgetSPtr& widget, unsigned index) :
        m_LayerIndexPtr { std::make_shared<unsigned>(index) } {

        m_RenderNodeMap.emplace(widget->m_WidgetId, std::make_shared<LRenderNode>(widget, true));
        m_RootNodePtr = m_RenderNodeMap[widget->m_WidgetId];
    }

    lshared_ptr<LRenderNode> tryEmplaceNodeMap(const LWidgetSPtr& widget) {
        m_RenderNodeMap.try_emplace(widget->m_WidgetId, std::make_shared<LRenderNode>(widget, true));
        return m_RenderNodeMap[widget->m_WidgetId];
    }

    loptional<lshared_ptr<LRenderNode>> node(long long id) {
        if (m_RenderNodeMap.find(id) != m_RenderNodeMap.end()) {
            return m_RenderNodeMap[id];
        }
        return std::nullopt;
    }

    lmap<long long, lshared_ptr<LRenderNode>> m_RenderNodeMap;
    lshared_ptr<LRenderNode>                  m_RootNodePtr;
    lshared_ptr<unsigned>                     m_LayerIndexPtr;
};

#endif // __LLAYERCONTEXT_HPP__
