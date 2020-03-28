#ifndef __LAPPLICATION_H__
#define __LAPPLICATION_H__

#include "Defines.hpp"
#include "base/framework/message_loop.h"
#include "base/thread/framework_thread.h"
#include "base/callback/callback.h"
#include "base/memory/lazy_instance.h"
#include "base/memory/singleton.h"

#define lApp (nbase::Singleton<LApplication>::GetInstance())

class LWindow;
using WindowVct = std::vector<LWindow*>;
class LApplication : public nbase::Dispatcher
{
    friend class nbase::Singleton<LApplication>;
public:
    virtual bool Dispatch(const MSG& msg);
    virtual ~LApplication() = default;

    void postTaskToUiThread(const StdClosure& closure);
    int exec();

    void addWindow(LWindow* wnd);
    void removeWindow(const LWindow* wnd);

private:
    LApplication();
    void onIdle();

private:
    nbase::FrameworkThread m_UiThread;
    WindowVct              m_WindowVct;
};

#endif // __LAPPLICATION_H__