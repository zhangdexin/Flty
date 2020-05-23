#include "LGraphicManager.h"
#include "LWidget.h"
#include "LLayerContext.h"

LGraphicManager::LGraphicManager()
{
    m_Surface = SkSurface::MakeRasterN32Premul(800, 600);
    //m_Bitmap.allocPixels();
}

void LGraphicManager::updateContexts(const lvct_shared_ptr<LLayerContext>& contexts)
{
    m_LayerContexts = contexts;
}

void LGraphicManager::needGraphic(const LWidgetSPtr& widget)
{
    auto& layerContext = m_LayerContexts[widget->layerIndex()];
    auto&& nodeptr = layerContext->node(widget->m_WidgetId);
    if (nodeptr && !nodeptr->get()->m_GraphicChangedByLayout) {
        nodeptr->get()->setStyle(widget->m_Style)
                      .setGraphicChangedBySet(true);
    }
}

void LGraphicManager::graphic(const lset<unsigned>& layers)
{
    for (auto &index : layers) {
        m_LayerContexts[index]->graphic();
    }

    compose();
}

void LGraphicManager::compose()
{
    SkCanvas* canvas = m_Surface->getCanvas();
    canvas->clear(SK_ColorWHITE);

    for (auto& context : m_LayerContexts) {
        canvas->drawImage(context->m_Surface->makeImageSnapshot(), 0, 0);
    }
}

void LGraphicManager::swapImage(sk_sp<SkImage>& image)
{
    image = m_Surface->makeImageSnapshot();
}