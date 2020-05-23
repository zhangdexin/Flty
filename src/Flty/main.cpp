#include <queue>
#include <list>
#include <iostream>
#include <type_traits>
#include <memory>

#include "Main.hpp"
#include "LWindow.h"
#include "LLabel.h"
#include "SkColor.h"
#include "LApplication.h"


#include <ShObjIdl_core.h>
#include <ShlGuid.h>
#include <ShlObj_core.h>

int Main(void* platformData, int argc, lstring *argv)
{
    LWindowSPtr win = std::make_shared<LWindow>(platformData);
    LWidgetSPtr widget{ new LWidget() };
    LWidgetSPtr widget1{ new LWidget() };

    widget1->setBackgroundColor(SK_ColorBLUE);
    widget1->setSize(SkSize::Make(100, 100));

    widget->setBackgroundColor(SK_ColorRED);
    widget->setSize(SkSize::Make(400, 200));

    win->addRootChild(widget);
    widget->addChildWidget(widget1);

    win->show();
    return lApp->exec();

}