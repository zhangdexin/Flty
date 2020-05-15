#ifndef __LGRAPHICMANAGER_H__
#define __LGRAPHICMANAGER_H__

#include "Defines.hpp"

class LLayerContext;
class LGraphicManager
{
public:
    enum Status: unsigned char {
        Status_Idle,
        Status_Drawing,
    };

    LGraphicManager() = default;

    bool isDrawing() const {
        return (m_Status == Status::Status_Drawing);
    }

    void updateContexts(const lvct_shared_ptr<LLayerContext>& contexts);

    void needGraphic(const LWidgetSPtr& widget);
    void graphic(const lset<unsigned>& layers);
    void swapBitmap(SkBitmap& bitmap);

private:
    void compose();

private:
    Status                         m_Status = Status::Status_Idle;
    lvct_shared_ptr<LLayerContext> m_LayoutContexts;
    SkBitmap                       m_Bitmap;
};

#endif // __LGRAPHICMANAGER_H__
