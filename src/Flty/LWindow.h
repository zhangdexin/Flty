#ifndef __LWINDOW_H__
#define __LWINDOW_H__

#include "Defines.hpp"
#include "LApplication.h"

class LWindow : public SkWindow::Layer
{
public:
    LWindow(void* platformData);
    ~LWindow();

    virtual void onPaint(SkSurface* surface);
    virtual void onBackendCreated();

    void onIdle();
    void show();

private:
    sk_app::Window*                 m_Window;
    sk_app::Window::BackendType     m_BeckendType;
};

#endif // __LWINDOW_H__

