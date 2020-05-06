#include "LLabel.h"

LLabel::LLabel(const LWidgetSPtr& widget):
    LWidget(widget)
{
}

LLabel::LLabel(const LWindowSPtr& window) :
    LWidget(window)
{
}
