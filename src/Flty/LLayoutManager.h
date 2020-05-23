#ifndef __LLAYOUTMANAGER_H__
#define __LLAYOUTMANAGER_H__

#include "Defines.hpp"

class LLayerContext;
class LRenderNode;
class LLayoutManager
{
public:
    LLayoutManager(const lshared_ptr<LLayerContext>& layer);

    void needLayout(const LWidgetSPtr& widget);
    void layout();

private:
    void doLayout(const lshared_ptr<LRenderNode>& parentNode,
                  const lshared_ptr<LRenderNode>& node);

private:
    lshared_ptr<LLayerContext> m_LayerContext;
};

#endif // __LLAYOUTMANAGER_H__
