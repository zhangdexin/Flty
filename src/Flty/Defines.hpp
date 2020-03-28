#ifndef __DEFINES_H__
#define __DEFINES_H__

#ifdef _WIN32
#include <Windows.h>
#include <tchar.h>
#endif

#include "tools/sk_app/Window.h"
#include "tools/sk_app/DisplayParams.h"

#include <string>
#include <memory>
#include <vector>

using tstring = std::wstring;
using lstring = std::string;
using tstringView = std::string_view;
using lstringView = std::wstring_view;

using SkWindow = sk_app::Window;
using SkDisplayParams = sk_app::DisplayParams;

#define LDEBUG 1

#endif
