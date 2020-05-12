#ifndef __LLAYOUTMANAGER_H__
#define __LLAYOUTMANAGER_H__

#include "Defines.hpp"
#include "WidgetRenderNode.hpp"

class LLayoutManager
{
public:
    LLayoutManager(const LWidgetSPtr& widget);

    void needLayout(const LWidgetSPtr& widget);
    void layout();

private:
    bool compareLayoutAndCopy(LStyleSheet& style);
    void doLayout(const LWidgetSPtr& parentWidget, const LWidgetSPtr& widget);

private:
    lmap<long long, WidgetRenderNode> m_RenderNodeMap;
    WidgetRenderNode                 *m_RootNode;
};

#endif // __LLAYOUTMANAGER_H__
