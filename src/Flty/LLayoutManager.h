#ifndef __LLAYOUTMANAGER_H__
#define __LLAYOUTMANAGER_H__

#include "Defines.hpp"

class LLayerContext;
class LLayoutManager
{
public:
    LLayoutManager(const lshared_ptr<LLayerContext>& layer);

    void needLayout(const LWidgetSPtr& widget);
    void layout();

private:
    void doLayout(const LWidgetSPtr& parentWidget, const LWidgetSPtr& widget);

private:
    lshared_ptr<LLayerContext> m_LayerContext;
};

#endif // __LLAYOUTMANAGER_H__
