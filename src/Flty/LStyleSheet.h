#ifndef __LSTYLESHEET_H__
#define __LSTYLESHEET_H__

#include "include/core/SkColor.h"

class LStyleSheet
{
public:
    void setBackgroundColor(const SkColor& color);

    constexpr unsigned width() const {
        return m_Width;
    }

    constexpr unsigned height() const {
        return m_Height;
    }

private:
    SkColor  m_BlackgroundColor = SK_ColorGRAY;
    unsigned m_Width = 200;
    unsigned m_Height = 40;
};

#endif //__LSTYLESHEET_H__
