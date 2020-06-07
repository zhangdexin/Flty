#ifndef __LSTYLESHEET_H__
#define __LSTYLESHEET_H__

#include "include/core/SkColor.h"
#include "include/core/SkSize.h"
#include "include/core/SkRect.h"
#include "include/core/SkPoint.h"

namespace DefaultStyle {

constexpr SkColor backgroundColor = SK_ColorGREEN;
const SkSize size = SkSize::Make(200, 40);
const SkRect rect = SkRect::MakeSize(DefaultStyle::size);

}

class LStyleSheet
{
public:
    void setBackgroundColor(const SkColor& color);
    void setSize(const SkSize& size);

    enum StyleType : unsigned char {
        StyleType_Size,
        StyleType_BackGround,
        StyleType_Pos,
        StyleType_Rect
    };

    SkSize size() const {
        return m_Size;
    }

    SkColor backgroundColor() const {
        return m_BlackgroundColor;
    }

    SkPoint pos() const {
        return SkPoint::Make(m_Rect.x(), m_Rect.y());
    }

    SkRect rect() const {
        return m_Rect;
    }

    void updateRect() {
        updateRect({0, 0});
    }
    void updateRect(const SkIPoint& point);
    void updateRectBy(const SkPoint& point);
    void updateRect(const SkIPoint& point, const SkSize& size);

    void compareLayoutAndCopy(LStyleSheet& style);

private:
    SkColor m_BlackgroundColor = DefaultStyle::backgroundColor;
    SkSize  m_Size             = DefaultStyle::size;
    SkRect  m_Rect             = DefaultStyle::rect;
};

#endif //__LSTYLESHEET_H__
