#include <queue>
#include <list>
#include <iostream>
#include <type_traits>
#include <memory>

#include "Main.hpp"
#include "LWindow.h"
#include "LLabel.h"
#include "LApplication.h"

#include <shlobj.h>

std::string Trim(std::string& str)
{
    std::string::size_type pos = str.find_last_not_of(' ');
    if (pos != std::string::npos)
    {
        str.erase(pos + 1);
        pos = str.find_first_not_of(' ');
        if (pos != std::string::npos)
            str.erase(0, pos);
    }
    else
        str.erase(str.begin(), str.end());
    return str;
}

int Main(void* platformData, int argc, lstring *argv)
{
    LWindowSPtr win = std::make_shared<LWindow>(platformData);
    LWidgetSPtr label{ new LLabel(win) };

    win->show();
    return lApp->exec();
}