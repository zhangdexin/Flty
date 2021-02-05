#include "LStyleSheet.h"

void LStyleSheet::setBackgroundColor(const SkColor& color)
{
    m_BlackgroundColor = color;
}

void LStyleSheet::setSize(const SkISize& size)
{
    m_Size = size;
}

void LStyleSheet::setPos(const SkIPoint& point)
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

void LStyleSheet::setBorder(const LBorder& border)
{
    m_Border = border;
}

void LStyleSheet::setBorderRadius(int radius)
{
    m_Border.m_BorderRadius = radius;
}

void LStyleSheet::setMargin(const std::initializer_list<int>& lt)
{
    if (lt.size() == 1) {
        int size = *lt.begin();
        m_Margin.m_Top = size;
        m_Margin.m_Left = size;
        m_Margin.m_Right = size;
        m_Margin.m_Bottom = size;
        return;
    }
    
    if (lt.size() == 4) {
        auto iter = lt.begin();
        m_Margin.m_Top = *iter++;
        m_Margin.m_Right = *iter++;
        m_Margin.m_Bottom = *iter++;
        m_Margin.m_Left = *iter++;
    }
}

void LStyleSheet::setPadding(const std::initializer_list<int>& lt)
{
    if (lt.size() == 1) {
        int size = *lt.begin();
        m_Padding.m_Top = size;
        m_Padding.m_Left = size;
        m_Padding.m_Right = size;
        m_Padding.m_Bottom = size;
        return;
    }

    if (lt.size() == 4) {
        auto iter = lt.begin();
        m_Padding.m_Top = *iter++;
        m_Padding.m_Right = *iter++;
        m_Padding.m_Bottom = *iter++;
        m_Padding.m_Left = *iter++;
    }
}

void LStyleSheet::setText(const lstring& text)
{
    m_Text = text;
}

void LStyleSheet::setTextColor(const SkColor& color)
{
    m_TextColor = color;
}

void LStyleSheet::updateContentRect()
{
    int bWidth = m_Border.m_Width;
    m_ContentRect = {
        Sk32_sat_add(m_BoundingRect.fLeft, m_Padding.m_Left + bWidth),
        Sk32_sat_add(m_BoundingRect.fTop, m_Padding.m_Top + bWidth),
        Sk32_sat_sub(m_BoundingRect.fRight, m_Padding.m_Right + bWidth),
        Sk32_sat_sub(m_BoundingRect.fBottom, m_Padding.m_Bottom + bWidth)
    };
}

void LStyleSheet::updateBoundingRect()
{
    m_BoundingRect.setXYWH(m_Pos.x(), m_Pos.y(), m_Size.width(), m_Size.height());
}

void LStyleSheet::updateBoundingRectByOffset(const SkIPoint& pos)
{
    updateBoundingRect();
    m_BoundingRect.offset(pos);
    m_Pos.set(m_BoundingRect.fLeft, m_BoundingRect.fTop);
}
