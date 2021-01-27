#ifndef __LLAYOUTMANAGER_H__
#define __LLAYOUTMANAGER_H__

#include "Defines.hpp"
#include "LStyleSheet.h"

class LLayerContext;
class LRenderNode;
class LLayoutManager
{
    using WidgetSizeChangedFunc = std::function<void(const LStyleSheet&, int)>;

public:
    LLayoutManager(const lshared_ptr<LLayerContext>& layer, WidgetSizeChangedFunc&& f);

    void needLayout(const lwidget_sptr& widget);
    void layout();

private:
    void doLayout(lshared_ptr<LRenderNode>& parent);

    void computeNoneBoxStyle(lshared_ptr<LRenderNode>& parent);
    void computeHorizontalStyle(lshared_ptr<LRenderNode>& parent);
    int calcHorizontalBoxHeight(const LStyleSheet& style, int limitHeight);

    void computeVerticalStyle(lshared_ptr<LRenderNode>& parent);
    int calcVerticalBoxWidth(const LStyleSheet& style, int limitWidth);

    void updateBounding(const SkIRect& parentBounding, LStyleSheet& style);

private:
    lshared_ptr<LLayerContext>      m_LayerContext;
    WidgetSizeChangedFunc           m_sizeChangedCb;
};

#endif // __LLAYOUTMANAGER_H__