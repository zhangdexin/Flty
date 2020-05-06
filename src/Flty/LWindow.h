#ifndef __LWINDOW_H__
#define __LWINDOW_H__

#include "Defines.hpp"
#include "LApplication.h"

class LWidget;
class LLayoutManager;
class LWindow : public SkWindow::Layer, public std::enable_shared_from_this<LWindow>
{
public:
    LWindow(void* platformData);
    ~LWindow();

    virtual void onPaint(SkSurface* surface);
    virtual void onBackendCreated();

    void onIdle();
    void show();

    void setTitle(const char* text);
    void addRootChild(const LWidgetSPtr& widget);

private:
    lunique_ptr<sk_app::Window>     m_WindowPtr;
    sk_app::Window::BackendType     m_BeckendType;

    lvct_shared_ptr<LWidget>        m_Roots;
    lvct_unique_ptr<LLayoutManager> m_LayoutMgrs;
};

#endif // __LWINDOW_H__

