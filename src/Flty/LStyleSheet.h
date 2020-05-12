#ifndef __LSTYLESHEET_H__
#define __LSTYLESHEET_H__

#include "include/core/SkColor.h"
#include "include/core/SkSize.h"
#include "include/core/SkRect.h"
#include "include/core/SkPoint.h"

class LStyleSheet
{
public:
    void setBackgroundColor(const SkColor& color);
    void setSize(const SkSize& size);

    SkSize size() const {
        return m_Size;
    }

    SkColor backgroundColor() const {
        return m_BlackgroundColor;
    }

    SkPoint pos() const {
        return SkPoint::Make(m_Rect.x(), m_Rect.y());
    }

    void updateRect() {
        updateRect({0, 0});
    }
    void updateRect(const SkIPoint& point);
    void updateRectBy(const SkPoint& point);
    void updateRect(const SkIPoint& point, const SkSize& size);

    bool compareLayoutAndCopy(LStyleSheet& style);

private:
    SkColor m_BlackgroundColor = SK_ColorGRAY;
    SkSize  m_Size             = SkSize::Make(200, 40);
    SkRect  m_Rect             = SkRect::MakeSize(m_Size);
};

#endif //__LSTYLESHEET_H__
