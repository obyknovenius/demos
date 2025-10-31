#pragma once

#include <Core/EventLoop.h>

namespace GUI
{
    class Application
    {
    public:
        static Application& get() { return _application; }

        void run();
        void quit();

    private:
        static Application _application;

        Application() = default;
        ~Application() = default;

        Core::EventLoop& _event_loop { Core::EventLoop::getMain() };
    };

    inline void Application::run()
    {
        _event_loop.run();
    }

    inline void Application::quit()
    {
        _event_loop.quit();
    }
}
