#include "LStyleSheet.h"

void LStyleSheet::setBackgroundColor(const SkColor& color)
{
    m_BlackgroundColor = color;
}

void LStyleSheet::setSize(const SkSize& size)
{
    m_Size = size;
    m_Rect = SkRect::MakeSize(m_Size);
}

void LStyleSheet::updateRect(const SkIPoint& point)
{
    m_Rect.setXYWH(point.x(), point.y(), m_Size.width(), m_Size.height());
}

void LStyleSheet::updateRectBy(const SkPoint& point)
{
    m_Rect.makeOffset(point);
}

void LStyleSheet::updateRect(const SkIPoint& point, const SkSize& size)
{
    m_Rect.setXYWH(point.x(), point.y(), size.width(), size.height());
    m_Size = size;
}

void LStyleSheet::compareLayoutAndCopy(LStyleSheet& style)
{
    if (style.m_Size != m_Size) {
        style.m_Size = m_Size;
    }

    if (style.m_Rect != m_Rect) {
        style.m_Rect = m_Rect;
    }
}
