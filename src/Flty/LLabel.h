#ifndef __LLABEL_H__
#define __LLABEL_H__

#include "LWidget.h"
#include "LWindow.h"

class LLabel: public LWidget
{
public:
    LLabel(const LWidgetSPtr& widget);
    LLabel(const LWindowSPtr& window);



};

#endif // __LLABEL_H__