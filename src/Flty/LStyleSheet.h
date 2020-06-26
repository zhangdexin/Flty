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
constexpr SkPoint pos = SkPoint::Make(0, 0);

}

class LStyleSheet
{
public:
    void setBackgroundColor(const SkColor& color);
    void setSize(const SkSize& size);
    void setPos(const SkPoint& point);

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
        return m_Pos;
    }

    SkRect boundingRect() const {
        return m_BoundingRect;
    }

    void updateBoundingRect();
    void updateBoundingRectBy(const SkPoint& pos);
    void compareLayoutAndCopy(LStyleSheet& style);


private:
    SkColor m_BlackgroundColor = DefaultStyle::backgroundColor;
    SkSize  m_Size             = DefaultStyle::size;
    SkPoint m_Pos              = DefaultStyle::pos;
    SkRect  m_BoundingRect     = DefaultStyle::rect;
};

#endif //__LSTYLESHEET_H__
