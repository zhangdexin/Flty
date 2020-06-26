#include "LStyleSheet.h"

void LStyleSheet::setBackgroundColor(const SkColor& color)
{
    m_BlackgroundColor = color;
}

void LStyleSheet::setSize(const SkSize& size)
{
    m_Size = size;
}

void LStyleSheet::setPos(const SkPoint & point)
{
    m_Pos = point;
}

void LStyleSheet::updateBoundingRect()
{
    m_BoundingRect.setXYWH(m_Pos.x(), m_Pos.y(), m_Size.width(), m_Size.height());
}

void LStyleSheet::updateBoundingRectBy(const SkPoint& pos)
{
    updateBoundingRect();
    m_BoundingRect.offset(pos);
}

void LStyleSheet::compareLayoutAndCopy(LStyleSheet& style)
{
    //if (style.m_Size != m_Size) {
    //    style.m_Size = m_Size;
    //}

    //if (style.m_Rect != m_Rect) {
    //    style.m_Rect = m_Rect;
    //}
}
