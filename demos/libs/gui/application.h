#pragma once

#include <core/event_loop.h>

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

        core::event_loop& m_event_loop { core::event_loop::get_main() };
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
