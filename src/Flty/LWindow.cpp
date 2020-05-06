#include "LWindow.h"

#include "include/core/SkGraphics.h"
#include "include/core/SkSurface.h"
#include "LLayoutManager.h"
#include "LWidget.h"

LWindow::LWindow(void* platformData) :
    m_BeckendType{ SkWindow::kNativeGL_BackendType },
    m_WindowPtr{ SkWindow::CreateNativeWindow(platformData)}
{
    m_WindowPtr->setRequestedDisplayParams(SkDisplayParams());
    m_WindowPtr->pushLayer(this);
    m_WindowPtr->attach(m_BeckendType);

    lApp->addPreExecQueue([this]() {
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
    canvas->clear(SK_ColorWHITE);

    SkPaint paint;
    paint.setColor(SK_ColorRED);

    SkRect rect = SkRect::MakeXYWH(100, 100, 100, 100);
    canvas->drawRect(rect, paint);
}

void LWindow::onBackendCreated()
{
    m_WindowPtr->setTitle("mainwidnow");
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
    m_Roots.push_back(widget);
    m_LayoutMgrs.emplace_back(std::make_unique<LLayoutManager>(widget));
    widget->setAttachWnd(shared_from_this());
}
