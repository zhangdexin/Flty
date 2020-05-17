#ifndef __LLAYERCONTEXT_HPP__
#define __LLAYERCONTEXT_HPP__

#include "Defines.hpp"
#include "LWidget.h"
#include "LRenderNode.hpp"

class SkCanvas;
class LLayerContext
{
public:
    LLayerContext(const LWidgetSPtr& widget, unsigned index);

    lshared_ptr<LRenderNode> tryEmplaceNodeMap(const LWidgetSPtr& widget);
    loptional<lshared_ptr<LRenderNode>> node(long long id);

    void graphic();
    void doChildGraphic(SkCanvas *canvas,
                        const lshared_ptr<LRenderNode>& parentNode,
                        const LWidgetSPtr& firstChildWidget);

    lmap<long long, lshared_ptr<LRenderNode>> m_RenderNodeMap;
    lshared_ptr<LRenderNode>                  m_RootNodePtr;
    lshared_ptr<unsigned>                     m_LayerIndexPtr;
    sk_sp<SkSurface>                          m_Surface;
};

#endif // __LLAYERCONTEXT_HPP__
