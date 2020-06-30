#ifndef __LLAYOUTMANAGER_H__
#define __LLAYOUTMANAGER_H__

#include "Defines.hpp"

class LLayerContext;
class LRenderNode;
class LLayoutManager
{
public:
    LLayoutManager(const lshared_ptr<LLayerContext>& layer);

    void needLayout(const lwidget_sptr& widget);
    void layout();

private:
    void doLayout(lshared_ptr<LRenderNode>& parent, lshared_ptr<LRenderNode>& node);

private:
    lshared_ptr<LLayerContext> m_LayerContext;
};

#endif // __LLAYOUTMANAGER_H__
