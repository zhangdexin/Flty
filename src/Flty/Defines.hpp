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
#include <tuple>
#include <set>
#include <map>

// flty using
using ltstring = std::wstring;
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
using lset_shared_ptr = std::set<std::shared_ptr<T>>;

template<typename K, typename V>
using lmap = std::map<K, V>;

template<typename T>
using lshared_ptr = std::shared_ptr<T>;

template<typename T1, typename T2>
using ltuple = std::tuple<T1, T2>;

template<typename T, size_t _Size>
using larray = std::array<T, _Size>;

class LWidget;
using lwidget_sptr = lshared_ptr<LWidget>;

class LWindow;
using lwindow_sptr = lshared_ptr<LWindow>;

template<typename T>
using lunique_ptr = std::unique_ptr<T>;

template<typename T>
using loptional = std::optional<T>;

//using loptional_null = std::nullopt;

template<typename T>
using lref_wrapper = std::reference_wrapper<T>;

template<typename F>
using lfunction = std::function<F>;

template<typename T, typename Container = std::deque<T>>
using lqueue = std::queue<T, Container>;

using lclosure = lfunction<void(void)>;
using lthread = std::thread;

class LStyleSheet;
using lstyleTask = lfunction<void(LStyleSheet&)>;

// skia using
using SkWindow = sk_app::Window;
using SkDisplayParams = sk_app::DisplayParams;

#define LDEBUG 1

#endif
