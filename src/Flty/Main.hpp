#ifndef __MAIN_HPP__
#define __MAIN_HPP__

#include "Defines.hpp"
#include "base/util/string_util.h"

int Main(void* platformData, int argc, lstring *argv);

#ifdef _WIN32
#if LDEBUG
int main(int argc, char**argv) {
    lstring argvStr[20];
    for (int i = 0; i < argc; ++i) {
        argvStr[i] = *argv;
    }

    return Main(GetModuleHandle(NULL), argc, argvStr);
}
#endif // LDEBUG

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine, int nCmdShow) {

    lstring argv[20];
    int argc = 0;
    TCHAR exename[1024];
    int exenameLen = GetModuleFileName(NULL, exename, 1024);
    (void)exenameLen;

#ifdef UNICODE
    argv[argc++] = nbase::UTF16ToUTF8(exename);
#else // UNICODE
    argv[argc++] = _strdup(exename);
#endif

    TCHAR* next;
    TCHAR* arg = _tcstok_s(lpCmdLine, _T(" "), &next);
    while (arg != NULL) {
#ifdef UNICODE
        argv[argc++] = nbase::UTF16ToUTF8(arg);
#else // UNICODE
        argv[argc++] = _strdup(arg);
#endif
        arg = _tcstok_s(NULL, _T(" "), &next);
    }

    return Main((void*)hInstance, argc, argv);
}
#endif // _WIN32

#endif // __MAIN_HPP__
