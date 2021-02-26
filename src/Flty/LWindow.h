#ifndef __LWINDOW_H__
#define __LWINDOW_H__

#include "Defines.hpp"
#include "LApplication.h"

class LWidget;
class LLayoutManager;
class LLayerContext;
class LGraphicManager;
class LStyleSheet;
class LWindow : public SkWindow::Layer, public std::enable_shared_from_this<LWindow>
{
public:
    LWindow(void* platformData);
    ~LWindow();

    virtual void onPaint(SkSurface* surface) override;
    virtual void onBackendCreated() override;
    virtual bool onMouse(int x, int y, skui::InputState inputState, skui::ModifierKey modifierKey) override;
    virtual void onResize(int width, int height) override;

    void onIdle();
    void show();

    void setTitle(const char* text);
    void addRootChild(const lwidget_sptr& widget);
    void onChildWidgetAdd(const lwidget_sptr& widget);
    void storeWidgets(const lwidget_sptr& widget);

    void doRender();
    void layout();
    void graphic();

    void addLayoutSet(const lwidget_sptr& widget);
    void addGraphicSet(const lwidget_sptr& widget);

    void onWidgetSizeChanged(const LStyleSheet& style, int id);

    SkISize wndSize() const {
        return m_WndSize;
    }

private:
    lunique_ptr<sk_app::Window>     m_WindowPtr;
    sk_app::Window::BackendType     m_BeckendType;

    lset<lwidget_sptr>              m_LayoutWidgetSet;
    lset<lwidget_sptr>              m_GraphicWidgetSet;

    lvct_shared_ptr<LLayoutManager> m_LayoutMgrs;
    lvct_shared_ptr<LLayerContext>  m_LayerContexts;
    lunique_ptr<LGraphicManager>    m_GraphicMgr;
    sk_sp<SkImage>                  m_Image;

    lvct_shared_ptr<LWidget>        m_Roots;
    lunorder_map<int, lwidget_sptr> m_Widgets;
    SkISize                         m_WndSize;
};

#endif // __LWINDOW_H__
