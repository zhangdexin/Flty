#ifndef __LLAYOUTMANAGER_H__
#define __LLAYOUTMANAGER_H__

#include "Defines.hpp"

class LWidget;
class LLayoutManager
{
public:
    LLayoutManager(const LWidgetSPtr& widget);

private:
    LWidgetSPtr m_RootWidget;
};

#endif // __LLAYOUTMANAGER_H__
