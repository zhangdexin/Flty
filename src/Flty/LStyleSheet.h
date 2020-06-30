#ifndef __LSTYLESHEET_H__
#define __LSTYLESHEET_H__

#include "include/core/SkColor.h"
#include "include/core/SkSize.h"
#include "include/core/SkRect.h"
#include "include/core/SkPoint.h"

enum LBoxType : unsigned char {
    None,
    Vertical,
    Horizontal,
};

namespace DefaultStyle {

constexpr SkColor backgroundColor = SK_ColorGREEN;
constexpr SkSize size             = SkSize::Make(200, 40);
constexpr SkRect rect             = SkRect::MakeSize(DefaultStyle::size);
constexpr SkPoint pos             = SkPoint::Make(0, 0);
constexpr bool isFloat            = false;
constexpr LBoxType boxType        = LBoxType::None;

}

class LStyleSheet
{
public:
    void setBackgroundColor(const SkColor& color);
    void setSize(const SkSize& size);
    void setPos(const SkPoint& point);
    void setFloat(bool isFloat);
    void setBoxType(LBoxType type);

    enum StyleType : unsigned char {
        StyleType_Size,
        StyleType_Background,
        StyleType_Pos,
        StyleType_Rect,
        StyleType_Float,
    };

    int width() const {
        return m_Size.width();
    }

    int height() const {
        return m_Size.height();
    }

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

    LBoxType boxType() const {
        return m_BoxType;
    }

    void updateBoundingRect();
    void updateBoundingRectByOffset(const SkPoint& pos);
    void updateBoundingRectByOffAndSize(const SkPoint& pos, const SkSize& size);
    void compareLayoutAndCopy(LStyleSheet& style);


private:
    SkColor  m_BlackgroundColor = DefaultStyle::backgroundColor;
    SkSize   m_Size             = DefaultStyle::size;
    SkPoint  m_Pos              = DefaultStyle::pos;
    SkRect   m_BoundingRect     = DefaultStyle::rect;
    bool     m_IsFloat          = DefaultStyle::isFloat;
    LBoxType m_BoxType          = DefaultStyle::boxType;
};

#endif //__LSTYLESHEET_H__
