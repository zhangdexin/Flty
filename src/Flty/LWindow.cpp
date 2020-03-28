#include "LWindow.h"

#include "include/core/SkGraphics.h"
#include "include/core/SkSurface.h"

LWindow::LWindow(void* platformData) :
    m_BeckendType{ SkWindow::kNativeGL_BackendType },
    m_Window { SkWindow::CreateNativeWindow(platformData) }
{
    m_Window->setRequestedDisplayParams(SkDisplayParams());
    m_Window->pushLayer(this);
    m_Window->attach(m_BeckendType);

    lApp->addWindow(this);
}

LWindow::~LWindow()
{
    lApp->removeWindow(this);
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
}

void LWindow::onIdle()
{
    m_Window->inval();
}

void LWindow::show()
{
    m_Window->show();
}
