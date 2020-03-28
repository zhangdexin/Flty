#include <iostream>

#include <queue>
#include <list>
#include <type_traits>

#include "Main.hpp"
#include "LWindow.h"
#include "LApplication.h"

int Main(void* platformData, int argc, lstring *argv)
{
    LWindow win(platformData);
    win.show();
    return lApp->exec();
}