#ifndef __LWINDOW_H__
#define __LWINDOW_H__

#include "Defines.hpp"
#include "LApplication.h"

class LWidget;
class LLayoutManager;
class LLayerContext;
class LGraphicManager;
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
    void onChildWidgetAdd(const LWidgetSPtr& widget, unsigned layerIndex);

    void doPrePaint();
    void layout();
    void graphic();

    void addLayoutSet(const LWidgetSPtr& widget);
    void addGraphicSet(const LWidgetSPtr& widget);

private:
    lunique_ptr<sk_app::Window>     m_WindowPtr;
    sk_app::Window::BackendType     m_BeckendType;

    lset<LWidgetSPtr>               m_LayoutWidgetSet;
    lset<LWidgetSPtr>               m_GraphicWidgetSet;

    lvct_shared_ptr<LLayoutManager> m_LayoutMgrs;
    lvct_shared_ptr<LLayerContext>  m_LayerContexts;
    lunique_ptr<LGraphicManager>    m_GraphicMgr;
    sk_sp<SkImage>                  m_Image;
};

#endif // __LWINDOW_H__

