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
    m_WindowPtr->inval();
}

void LWindow::show()
{
    m_WindowPtr->show();
}

void LWindow::setTitle(const char* text)
{
    m_WindowPtr->setTitle(text);
}

void LWindow::addRootChild(const LWidgetSPtr& widget)
{
    if (widget->parent()) {
        return;
    }

    m_LayerContexts.emplace_back(std::make_shared<LLayerContext>(widget, m_LayerContexts.size()));
    m_LayoutMgrs.emplace_back(std::make_shared<LLayoutManager>(m_LayerContexts.back()));

    widget->setAttachWnd(shared_from_this());
    widget->setLayerIndex(m_LayerContexts.back()->m_LayerIndexPtr);

    addLayoutSet(widget);
    addGraphicSet(widget);
}

void LWindow::onChildWidgetAdd(const LWidgetSPtr& widget, unsigned layerIndex)
{
    if (!widget->parent()) {
        return;
    }

    m_LayerContexts[layerIndex]->addChildNode(widget);

    addLayoutSet(widget);
    addGraphicSet(widget);
}

void LWindow::addLayoutSet(const LWidgetSPtr& widget)
{
    m_LayoutWidgetSet.insert(widget);
}

void LWindow::addGraphicSet(const LWidgetSPtr& widget)
{
    m_GraphicWidgetSet.insert(widget);
}

void LWindow::doPrePaint()
{
    if (m_LayerContexts.size() > 0 && m_LayoutMgrs.size() > 0) {
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
}
