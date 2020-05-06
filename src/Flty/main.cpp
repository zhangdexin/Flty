#include <queue>
#include <list>
#include <iostream>
#include <type_traits>
#include <memory>

#include "Main.hpp"
#include "LWindow.h"
#include "LLabel.h"
#include "LApplication.h"

int Main(void* platformData, int argc, lstring *argv)
{
    LWindowSPtr win = std::make_shared<LWindow>(platformData);
    LWidgetSPtr label{ new LLabel(win) };

    win->show();
    return lApp->exec();
}