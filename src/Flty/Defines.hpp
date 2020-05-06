#ifndef __DEFINES_H__
#define __DEFINES_H__

#ifdef _WIN32
#include <Windows.h>
#include <tchar.h>
#endif

#include "tools/sk_app/Window.h"
#include "tools/sk_app/DisplayParams.h"

#include <functional>
#include <optional>
#include <string>
#include <memory>
#include <vector>
#include <thread>
#include <queue>
#include <set>

// flty using
using tstring = std::wstring;
using lstring = std::string;
using tstringView = std::string_view;
using lstringView = std::wstring_view;

template<typename T>
using lvct = std::vector<T>;

template<typename T>
using lvct_shared_ptr = std::vector<std::shared_ptr<T>>;

template<typename T>
using lvct_unique_ptr = std::vector<std::unique_ptr<T>>;

template<typename T>
using lset = std::set<T>;

template<typename T>
using lset_ptr = std::set<std::shared_ptr<T>>;

template<typename T>
using lshared_ptr = std::shared_ptr<T>;

class LWidget;
using LWidgetSPtr = lshared_ptr<LWidget>;

class LWindow;
using LWindowSPtr = lshared_ptr<LWindow>;

template<typename T>
using lunique_ptr = std::unique_ptr<T>;

template<typename T>
using loptional = std::optional<T>;

template<typename F>
using lfunction = std::function<F>;

template<typename T, typename Container = std::deque<T>>
using lqueue = std::queue<T, Container>;

using LClosure = lfunction<void(void)>;
using lthread = std::thread;

// skia using
using SkWindow = sk_app::Window;
using SkDisplayParams = sk_app::DisplayParams;

#define LDEBUG 1

#endif
