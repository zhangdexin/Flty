#include "LStyleSheet.h"

void LStyleSheet::setBackgroundColor(const SkColor& color)
{
    m_BlackgroundColor = color;
}

void LStyleSheet::setSize(const SkISize& size)
{
    m_Size = size;
}

void LStyleSheet::setPos(const SkIPoint & point)
{
    m_Pos = point;
}

void LStyleSheet::setFloat(bool isFloat)
{
    m_IsFloat = isFloat;
}

void LStyleSheet::setBoxType(LBoxType type)
{
    m_BoxType = type;
}

void LStyleSheet::setMinSize(const SkISize& size)
{
    m_MinSize = size;
}

void LStyleSheet::setMaxSize(const SkISize& size)
{
    m_MaxSize = size;
}

void LStyleSheet::setFixedSize(const SkISize& size)
{
    setSize(size);
    m_FixedSizeType = LSizeFixed::AllFixed;
}

void LStyleSheet::setFixedWidth(int width)
{
    m_Size.fWidth = width;
    m_FixedSizeType |= LSizeFixed::WidthFixed;
}

void LStyleSheet::setFixedHeight(int height)
{
    m_Size.fHeight = height;
    m_FixedSizeType |= LSizeFixed::HeightFixed;
}

void LStyleSheet::setSizePolicy(LSizePolicy widthPolicy, LSizePolicy heightPolicy)
{
    m_WidthPolicy = widthPolicy;
    m_HeightPolicy = heightPolicy;
}

void LStyleSheet::updateBoundingRect()
{
    m_BoundingRect.setXYWH(m_Pos.x(), m_Pos.y(), m_Size.width(), m_Size.height());
}

void LStyleSheet::updateBoundingRectByOffset(const SkIPoint& pos)
{
    updateBoundingRect();
    m_BoundingRect.offset(pos);
}
