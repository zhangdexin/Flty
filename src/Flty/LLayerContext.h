#ifndef __LLAYERCONTEXT_HPP__
#define __LLAYERCONTEXT_HPP__

#include "Defines.hpp"
#include "LWidget.h"
#include "LRenderNode.hpp"

class SkCanvas;
class SkFont;
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
    loptional<lshared_ptr<LRenderNode>> node(int id);
    void appendLayerContextNode(const lshared_ptr<LLayerContext>& contextPtr, int parentId);

    void graphic();
    void doChildGraphic(SkCanvas *canvas, const lshared_ptr<LRenderNode>& node);
    void paintBody(const LStyleSheet& style, SkCanvas* canvas);
    void paintBorder(const LBorder& border, SkPaint& paint);
    void paintText(const LStyleSheet& style, SkCanvas* canvas);

    SkIRect validBoundRect() const;

    lmap<int, lshared_ptr<LRenderNode>>       m_RenderNodeMap;
    lshared_ptr<LRenderNode>                  m_RootNodePtr;
    lshared_ptr<unsigned>                     m_LayerIndexPtr;
    sk_sp<SkSurface>                          m_Surface;
};

#endif // __LLAYERCONTEXT_HPP__
