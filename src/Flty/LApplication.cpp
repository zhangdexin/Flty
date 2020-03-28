#include "LApplication.h"

#include "LWindow.h"

LApplication::LApplication():
    m_UiThread("Flty_Ui_Thread")
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

void LApplication::postTaskToUiThread(const StdClosure& closure)
{
    m_UiThread.message_loop()->PostTask(closure);
}

int LApplication::exec()
{
    m_UiThread.RunOnCurrentThreadWithLoop(nbase::MessageLoop::kUIMessageLoop, this);
    return 0;
}

void LApplication::addWindow(LWindow* wnd)
{
    m_WindowVct.push_back(wnd);
}

void LApplication::removeWindow(const LWindow* wnd)
{
    auto iter = std::find(m_WindowVct.begin(), m_WindowVct.end(), wnd);
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
