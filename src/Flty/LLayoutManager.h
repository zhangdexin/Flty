#ifndef __LLAYOUTMANAGER_H__
#define __LLAYOUTMANAGER_H__

#include "Defines.hpp"
#include "LStyleSheet.h"

class LLayerContext;
class LRenderNode;
class LLayoutManager
{
public:
    LLayoutManager(const lshared_ptr<LLayerContext>& layer);

    void needLayout(const lwidget_sptr& widget);
    void layout();

private:
    void doLayout(lshared_ptr<LRenderNode>& parent);

    void computeNoneBoxStyle(lshared_ptr<LRenderNode>& parent);
    void computeHorizontalStyle(lshared_ptr<LRenderNode>& parent);
    void computeVerticalStyle(lshared_ptr<LRenderNode>& parent);

    void updateBounding(const SkIRect& parentBounding, LStyleSheet& style);
    void doChildLayout(lshared_ptr<LRenderNode>& node);

    

    int computeHorizontalStyleOfAverage(LStyleSheet& style, int startOff, int everyWidth, int limitWidth, int limitHeight);
    int computeHorizontalStyleOfHint(LStyleSheet& style, int startOff, int everyWidth, int limitWidth, int limitHeight);
    void computeVerticalStyle(LStyleSheet& style, int index, int width, int childHeight);

    SkISize computeSizeByPolicy(const LStyleSheet& style, const SkISize& limitSize);
    ltuple<larray<int, LSizePolicy::Count>, int> getChildrenPolicyNumsAndExpandingSize(
            const lshared_ptr<LRenderNode>& parent);

private:
    lshared_ptr<LLayerContext> m_LayerContext;
};

#endif // __LLAYOUTMANAGER_H__
