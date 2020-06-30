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
    lwidget_sptr widget2{ new LWidget() };

    widget2->setBackgroundColor(SK_ColorGREEN);
    widget2->setSize(SkSize::Make(300, 600));
    widget2->setPosition(SkPoint::Make(100, 100));
     
    widget1->setBackgroundColor(SK_ColorBLUE);
    widget1->setSize(SkSize::Make(800, 100));
    widget1->setPosition(SkPoint::Make(100, 120));

    widget->setBackgroundColor(SK_ColorRED);
    widget->setSize(SkSize::Make(800, 600));

    widget->addChildWidget(widget1);
    widget->addChildWidget(widget2);
    widget->setBox(LBoxType::Horizontal);
    win->addRootChild(widget);

    win->show();
    return lApp->exec();
}