#include "LLabel.h"

LLabel::LLabel()
{
}

void LLabel::setText(const lstring& text)
{
    SET_WIDGET_STYLE_ONE(text, setText, addGraphicSet);
}

void LLabel::setTextColor(const SkColor& color)
{
    SET_WIDGET_STYLE_ONE(color, setTextColor, addGraphicSet);
}
