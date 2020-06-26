#ifndef __LGRAPHICMANAGER_H__
#define __LGRAPHICMANAGER_H__

#include "Defines.hpp"
#include "SkSurface.h"

class LLayerContext;
class LGraphicManager
{
public:
    enum Status: unsigned char {
        Status_Idle,
        Status_Drawing,
    };

    LGraphicManager();
    void initCanvas(int width, int height);
    //void resizeCanvas(int width, int height);

    bool isDrawing() const {
        return (m_Status == Status::Status_Drawing);
    }

    void updateContexts(const lvct_shared_ptr<LLayerContext>& contexts);

    void needGraphic(const lwidget_sptr& widget);
    void graphic(const lset<unsigned>& layers);
    void swapImage(sk_sp<SkImage>& bitmap);

private:
    void compose();

private:
    Status                         m_Status = Status::Status_Idle;
    lvct_shared_ptr<LLayerContext> m_LayerContexts;
    sk_sp<SkSurface>               m_Surface;
};

#endif // __LGRAPHICMANAGER_H__
