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

#include <process.h>
#include <bcrypt.h>
#include <TlHelp32.h>
#include <Psapi.h>

int Main(void* platformData, int argc, lstring *argv)
{
    lwindow_sptr win = std::make_shared<LWindow>(platformData);
    lwidget_sptr widget{ new LWidget() };
    lwidget_sptr widget1{ new LWidget() };

    widget1->setBackgroundColor(SK_ColorBLUE);
    widget1->setSize(SkSize::Make(100, 100));
    widget1->setPosition(SkPoint::Make(20, 20));

    widget->setBackgroundColor(SK_ColorRED);
    widget->setSize(SkSize::Make(400, 200));

    win->addRootChild(widget);
    win->addRootChild(widget1);
    //widget->addChildWidget(widget1);

    win->show();
    return lApp->exec();
}