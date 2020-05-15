#include "LGraphicManager.h"

void LGraphicManager::updateContexts(const lvct_shared_ptr<LLayerContext>& contexts)
{
    m_LayoutContexts = contexts;
}

void LGraphicManager::needGraphic(const LWidgetSPtr & widget)
{
}

void LGraphicManager::graphic(const lset<unsigned>& layers)
{

}

void LGraphicManager::compose()
{
}

void LGraphicManager::swapBitmap(SkBitmap& bitmap)
{
    bitmap.swap(m_Bitmap);
}