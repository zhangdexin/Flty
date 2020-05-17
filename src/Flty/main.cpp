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


int Main(void* platformData, int argc, lstring *argv)
{
    LWindowSPtr win = std::make_shared<LWindow>(platformData);
    LWidgetSPtr widget{ new LWidget(win) };
    LWidgetSPtr widget1{ new LWidget(widget) };

    lApp->postTaskToMainThread([widget, widget1]() {

        widget1->setBackgroundColor(SK_ColorBLACK);
        widget1->setSize(SkSize::Make(100, 100));

        widget->setBackgroundColor(SK_ColorBLUE);
        widget->setSize(SkSize::Make(400, 200));
    });

    win->show();
    return lApp->exec();
}