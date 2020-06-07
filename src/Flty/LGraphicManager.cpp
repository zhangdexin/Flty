#include "LGraphicManager.h"
#include "LWidget.h"
#include "LLayerContext.h"

LGraphicManager::LGraphicManager()
{
    m_Surface = SkSurface::MakeRasterN32Premul(800, 600);
}

void LGraphicManager::updateContexts(const lvct_shared_ptr<LLayerContext>& contexts)
{
    m_LayerContexts = contexts;
}

void LGraphicManager::needGraphic(const lwidget_sptr& widget)
{
    auto& layerContext = m_LayerContexts[widget->layerIndex()];
    auto&& nodePtr = layerContext->node(widget->m_WidgetId);
    if (nodePtr && !nodePtr->get()->m_GraphicChangedByLayout) {

        auto &&node = nodePtr->get();
        auto &style = node->m_Style;

        node->setGraphicChangedBySet(true);

        auto &queue = widget->m_StyledChangedQueue;
        while (!queue->empty()) {
            lstyleTask task;
            queue->read(task);
            task(style);
        }
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