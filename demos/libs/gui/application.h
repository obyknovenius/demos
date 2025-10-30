#pragma once

#include <Core/EventLoop.h>

namespace gui
{
    class application
    {
    public:
        static application& get() { return s_application; }

        application() = default;
        ~application() = default;

        inline void run();
        inline void quit();

    private:
        static application s_application;

        Core::EventLoop& m_event_loop { Core::EventLoop::getMain() };
    };

    void application::run()
    {
        m_event_loop.run();
    }

    void application::quit()
    {
        m_event_loop.quit();
    }
}
