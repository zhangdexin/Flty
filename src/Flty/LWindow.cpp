#include "LWindow.h"

#include "include/core/SkGraphics.h"
#include "include/core/SkSurface.h"
#include "LLayoutManager.h"
#include "LLayerContext.h"
#include "LGraphicManager.h"
#include "LWidget.h"

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
    m_Roots.push_back(std::move(widget));
    widget->setAttachWnd(shared_from_this());

    lApp->postTaskToRenderThread([this, widget, 
                                  context = std::make_shared<LLayerContext>(widget)]() {

        m_LayerContexts.emplace_back(std::move(context));

        auto &&backLayer = m_LayerContexts.back();
        backLayer->setLayerIndex(m_LayerContexts.size() - 1);

        m_LayoutMgrs.emplace_back(std::make_shared<LLayoutManager>(backLayer));
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

    lApp->postTaskToRenderThread([this, widget, layerIndex = parent->layerIndex(),
                                  parentId = parent->m_WidgetId, wid = widget->m_WidgetId,
                                  context = std::make_shared<LLayerContext>(widget)]() {

        m_LayerContexts[layerIndex]->appendLayerContextNode(context, parentId);
        m_LayoutWidgetSet.insert(widget);
        m_GraphicWidgetSet.insert(widget);
    });
}

void LWindow::addLayoutSet(const lwidget_sptr& widget)
{
    lApp->postTaskToRenderThread([this, widget]() {
        m_LayoutWidgetSet.insert(widget);
    });
}

void LWindow::addGraphicSet(const lwidget_sptr& widget)
{
    lApp->postTaskToRenderThread([this, widget]() {
        m_GraphicWidgetSet.insert(widget);
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
