#include "LApplication.h"

#include "LWindow.h"

LApplication::LApplication():
    m_MainThread("Flty_Main_Thread"),
    m_ReanderThread("Flty_Render_Thread")
{}

bool LApplication::Dispatch(const MSG& msg)
{
    static bool idled = false;
    if (msg.hwnd == nullptr) {
        onIdle();
        idled = true;
        return true;
    }

    TranslateMessage(&msg);
    if (WM_PAINT == msg.message) {
        // Ensure that call onIdle at least once per WM_PAINT, or mouse events can
        // overwhelm the message processing loop, and prevent animation from running.
        if (!idled) {
            onIdle();
        }
        idled = false;
    }
    DispatchMessage(&msg);
    return true;
}

void LApplication::postTaskToMainThread(const LClosure& closure)
{
    m_MainThread.message_loop()->PostTask(closure);
}

int LApplication::exec()
{
    lthread thd{ std::bind(&LApplication::pollPreExecTask, this) };
    m_MainThread.RunOnCurrentThreadWithLoop(nbase::MessageLoop::kUIMessageLoop, this);

    thd.join();
    return 0;
}

void LApplication::addPreExecQueue(const LClosure& closure)
{
    m_PreExecQueue.push(closure);
}

void LApplication::pollPreExecTask()
{
    while (1) {
        if (m_MainThread.IsRunning() && m_MainThread.message_loop()) {
            while (m_PreExecQueue.size() > 0) {
                postTaskToMainThread(m_PreExecQueue.front());
                m_PreExecQueue.pop();
            }
            break;
        }
        std::this_thread::yield();
    }
}

void LApplication::addWindow(const LWindowSPtr& window)
{
    m_WindowVct.push_back(window);
}

void LApplication::removeWindow(const LWindowSPtr& window)
{
    auto iter = std::find(m_WindowVct.begin(), m_WindowVct.end(), window);
    if (iter != m_WindowVct.end()) {
        m_WindowVct.erase(iter);
    }
}

void LApplication::onIdle()
{
    for (auto& item : m_WindowVct) {
        item->onIdle();
    }
}
