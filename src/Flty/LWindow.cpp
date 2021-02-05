#include "LWindow.h"

#include "include/core/SkGraphics.h"
#include "include/core/SkSurface.h"
#include "LLayoutManager.h"
#include "LLayerContext.h"
#include "LGraphicManager.h"
#include "LWidget.h"
#include "LMouseEvent.h"

LWindow::LWindow(void* platformData) :
    m_BeckendType{ SkWindow::kNativeGL_BackendType },
    m_WindowPtr{ SkWindow::CreateNativeWindow(platformData)},
    m_GraphicMgr{ std::make_unique<LGraphicManager>() }
{
    m_WindowPtr->setRequestedDisplayParams(SkDisplayParams());
    m_WindowPtr->pushLayer(this);
    m_WindowPtr->attach(m_BeckendType);

    lApp->postTaskToMainThread([this]() {
        lApp->addWindow(shared_from_this());
    });
}

LWindow::~LWindow()
{
    lApp->removeWindow(shared_from_this());
}

void LWindow::onPaint(SkSurface* surface)
{
    auto canvas = surface->getCanvas();
    canvas->drawImage(m_Image, 0, 0);
}

void LWindow::onBackendCreated()
{
    m_WindowPtr->setTitle(u8"mainwidnow");
}

bool LWindow::onMouse(int x, int y, skui::InputState inputState, skui::ModifierKey modifierKey)
{
    if (modifierKey != skui::ModifierKey::kNone) {
        return false;
    }

    if (inputState == skui::InputState::kMove) {
        return false;
    }

    lshared_ptr<LMouseEvent> ev = std::make_shared<LMouseEvent>(x, y);
    ev->setMouseEventType(inputState);

    bool isHandled = false;
    int size = m_Roots.size();
    for (int i = size - 1; i >= 0 && !isHandled; --i) {
        if (m_Roots[i]->isContainsPt(ev->pos())) {
            isHandled = m_Roots[i]->event(ev);
        }
    }

    return isHandled;
}

void LWindow::onIdle()
{
    lApp->postTaskToRenderThread(std::bind(&LWindow::doRender, this));
    m_WindowPtr->inval();
}

void LWindow::onResize(int width, int height)
{
    m_GraphicMgr->initCanvas(width, height);
    for (auto& iter : m_LayerContexts) {
        iter->initCanvas(width, height);
    }

    for (auto &iter : m_Roots) {
        addLayoutSet(iter);
    }
}

void LWindow::show()
{
    m_WindowPtr->show();
}

void LWindow::setTitle(const char* text)
{
    m_WindowPtr->setTitle(text);
}

void LWindow::addRootChild(const lwidget_sptr& widget)
{
    if (widget->parent()) {
        return;
    }
    widget->setAttachWnd(shared_from_this());
    m_Roots.push_back(widget);
    storeWidgets(widget);

    lApp->postTaskToRenderThread([this, widget,
                                  context = std::make_shared<LLayerContext>(widget)]() {

        m_LayerContexts.emplace_back(std::move(context));

        auto &&backLayer = m_LayerContexts.back();
        backLayer->setLayerIndex(m_LayerContexts.size() - 1);

        using std::placeholders::_1;
        using std::placeholders::_2;

        m_LayoutMgrs.emplace_back(
            std::make_shared<LLayoutManager>(backLayer, std::bind(&LWindow::onWidgetSizeChanged, this, _1, _2)));
        widget->setLayerIndex(backLayer->m_LayerIndexPtr);

        m_LayoutWidgetSet.insert(widget);
        m_GraphicWidgetSet.insert(widget);
    });
}

void LWindow::onChildWidgetAdd(const lwidget_sptr& widget)
{
    auto &&parent = widget->parent();
    if (parent == nullptr) {
        return;
    }
    storeWidgets(widget);

    lApp->postTaskToRenderThread([this, widget, layerIndex = parent->layerIndex(),
                                  parentId = parent->m_WidgetId, wid = widget->m_WidgetId,
                                  context = std::make_shared<LLayerContext>(widget)]() {

        m_LayerContexts[layerIndex]->appendLayerContextNode(context, parentId);
        m_LayoutWidgetSet.insert(widget);
        m_GraphicWidgetSet.insert(widget);
    });
}

void LWindow::storeWidgets(const lwidget_sptr& widget)
{
    if (m_Widgets.find(widget->m_WidgetId) == m_Widgets.end()) {
        m_Widgets.emplace(widget->m_WidgetId, widget);
    }

    for (auto child : widget->children()) {
        storeWidgets(child);
    }
}

void LWindow::addLayoutSet(const lwidget_sptr& widget)
{
    lApp->postTaskToRenderThread([this, widget]() {
        m_LayoutWidgetSet.insert(widget);
        m_GraphicWidgetSet.insert(widget);
    });
}

void LWindow::addGraphicSet(const lwidget_sptr& widget)
{
    lApp->postTaskToRenderThread([this, widget]() {
        m_GraphicWidgetSet.insert(widget);
    });
}

void LWindow::onWidgetSizeChanged(const LStyleSheet& style, int id)
{
    lApp->postTaskToMainThread([this, id, style]() {
        if (m_Widgets.find(id) != m_Widgets.end()) {
            m_Widgets[id]->m_Style.setPos(style.pos());
            m_Widgets[id]->m_Style.setSize(style.size());
        }
    });
}

void LWindow::doRender()
{
    if (m_LayoutWidgetSet.size() > 0 || m_GraphicWidgetSet.size() > 0) {
        layout();
        graphic();
    }
}

void LWindow::layout()
{
    lset<unsigned> layers;
    for (auto &item : m_LayoutWidgetSet) {
        m_LayoutMgrs[item->layerIndex()]->needLayout(item);
        layers.insert(item->layerIndex());
    }

    for (auto &item : layers) {
        m_LayoutMgrs[item]->layout();
    }
    m_LayoutWidgetSet.clear();
}

void LWindow::graphic()
{
    m_GraphicMgr->updateContexts(m_LayerContexts);

    lset<unsigned> layers;
    for (auto &item : m_GraphicWidgetSet) {
        m_GraphicMgr->needGraphic(item);
        layers.insert(item->layerIndex());
    }

    m_GraphicMgr->graphic(layers);
    m_GraphicMgr->swapImage(m_Image);

    m_GraphicWidgetSet.clear();
}
