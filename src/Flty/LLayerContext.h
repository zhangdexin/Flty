#ifndef __LLAYERCONTEXT_HPP__
#define __LLAYERCONTEXT_HPP__

#include "Defines.hpp"
#include "LWidget.h"
#include "LRenderNode.hpp"

class SkCanvas;
class LStyleSheet;
class LLayerContext
{
public:
    LLayerContext() = default;
    LLayerContext(const lwidget_sptr& widget);

    void initCanvas(int width, int height);
    void setLayerIndex(unsigned index);

    void addRootNode(const lwidget_sptr& widget);
    void addChildNode(const lwidget_sptr& widget);
    loptional<lshared_ptr<LRenderNode>> node(long long id);
    void appendLayerContextNode(const lshared_ptr<LLayerContext>& contextPtr, long long parentId);

    void graphic();
    void doChildGraphic(SkCanvas *canvas, const lshared_ptr<LRenderNode>& node);
    void paintBody(const LStyleSheet& style, SkCanvas* canvas);
    void paintBorder(const LBorder& border, SkPaint& paint);

    SkIRect validBoundRect() const;

    lmap<long long, lshared_ptr<LRenderNode>> m_RenderNodeMap;
    lshared_ptr<LRenderNode>                  m_RootNodePtr;
    lshared_ptr<unsigned>                     m_LayerIndexPtr;
    sk_sp<SkSurface>                          m_Surface;
};

#endif // __LLAYERCONTEXT_HPP__
