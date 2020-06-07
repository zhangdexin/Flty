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
class LApplication : public nbase::Dispatcher
{
    friend class nbase::Singleton<LApplication>;
public:
    virtual bool Dispatch(const MSG& msg);
    virtual ~LApplication() = default;

    void postTaskToMainThread(const lclosure& closure);
    void postTaskToRenderThread(const lclosure& closure);
    int exec();

    void addWindow(const lwindow_sptr& window);
    void removeWindow(const lwindow_sptr& window);

private:
    LApplication();
    void onIdle();

private:
    nbase::FrameworkThread   m_MainThread;
    nbase::FrameworkThread   m_ReanderThread;
    lvct_shared_ptr<LWindow> m_WindowVct;
    lqueue<lclosure>         m_PreExecQueue;
};

#endif // __LAPPLICATION_H__