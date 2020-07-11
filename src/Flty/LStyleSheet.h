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

enum LSizePolicy : unsigned char {
    Hint      = 0,
    Expanding = 1,
    Count     = 2,
};

enum LSizeFixed : unsigned short {
    NoneFixed   = 0x00,
    WidthFixed  = 0x01,
    HeightFixed = 0x02,
    AllFixed    = WidthFixed | HeightFixed,
};

namespace DefaultStyle {

constexpr SkColor backgroundColor = SK_ColorGREEN;
constexpr SkISize size            = SkISize::Make(200, 40);
constexpr SkISize maxSize         = SkISize::Make(INT16_MAX, INT16_MAX);
constexpr SkIRect rect            = SkIRect::MakeSize(size);
constexpr SkIPoint pos            = SkIPoint::Make(0, 0);
constexpr LBoxType boxType        = LBoxType::None;
constexpr LSizePolicy sizePolicy  = LSizePolicy::Hint;
constexpr LSizeFixed sieFixed     = LSizeFixed::NoneFixed;

}

class LStyleSheet
{
public:
    enum StyleType : unsigned char {
        StyleType_Size,
        StyleType_Background,
        StyleType_Pos,
        StyleType_Rect,
        StyleType_Float,
    };

    void setBackgroundColor(const SkColor& color);
    void setSize(const SkISize& size);
    void setMinSize(const SkISize& size);
    void setMaxSize(const SkISize& size);
    void setFixedSize(const SkISize& size);
    void setFixedWidth(int width);
    void setFixedHeight(int height);
    void setPos(const SkIPoint& point);
    void setFloat(bool isFloat);
    void setBoxType(LBoxType type);
    void setSizePolicy(LSizePolicy widthPolicy, LSizePolicy heightPolicy);

    int width() const {
        return m_Size.width();
    }

    int height() const {
        return m_Size.height();
    }

    SkISize size() const {
        return m_Size;
    }

    SkColor backgroundColor() const {
        return m_BlackgroundColor;
    }

    SkIPoint pos() const {
        return m_Pos;
    }

    SkIRect boundingRect() const {
        return m_BoundingRect;
    }

    LBoxType boxType() const {
        return m_BoxType;
    }

    SkISize minSize() const {
        return m_MinSize;
    }

    SkISize maxSize() const {
        return m_MaxSize;
    }

    LSizePolicy widthPolicy() const {
        return m_WidthPolicy;
    }

    LSizePolicy heightPolicy() const {
        return m_HeightPolicy;
    }

    bool isWidthFixed() const {
        return m_FixedSizeType & LSizeFixed::WidthFixed;
    }

    bool isHeightFixed() const {
        return m_FixedSizeType & LSizeFixed::HeightFixed;
    }

    void updateBoundingRect();
    void updateBoundingRectByOffset(const SkIPoint& pos);


private:
    SkColor     m_BlackgroundColor = DefaultStyle::backgroundColor;
    SkISize     m_Size             = DefaultStyle::size;
    SkISize     m_MinSize          = DefaultStyle::size;
    SkISize     m_MaxSize          = DefaultStyle::maxSize;
    SkIPoint    m_Pos              = DefaultStyle::pos;
    SkIRect     m_BoundingRect     = DefaultStyle::rect;
    bool        m_IsFloat          = false;
    int         m_FixedSizeType    = DefaultStyle::sieFixed;
    LBoxType    m_BoxType          = DefaultStyle::boxType;
    LSizePolicy m_WidthPolicy      = DefaultStyle::sizePolicy;
    LSizePolicy m_HeightPolicy     = DefaultStyle::sizePolicy;
};

#endif //__LSTYLESHEET_H__
