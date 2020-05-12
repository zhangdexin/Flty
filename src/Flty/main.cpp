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
int Main(void* platformData, int argc, lstring *argv)
{
    LWindowSPtr win = std::make_shared<LWindow>(platformData);
    LWidgetSPtr label{ new LLabel(win) };

    win->show();
    return lApp->exec();

    //std::wstring sHomePath(_wgetenv(L"HOMEPATH"));
    //sHomePath += L"\\Documents";

    //char szDir[MAX_PATH] = {};
    //GetSystemDirectoryA(szDir, MAX_PATH);

    //wchar_t szBuf[1025] = { 0 };
    //SHGetSpecialFolderPathW(NULL, szBuf, CSIDL_STARTMENU, TRUE);

    //return 0;
}