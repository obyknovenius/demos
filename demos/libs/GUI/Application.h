#pragma once

#include <Core/EventLoop.h>
#include <Core/NonNull.h>

namespace GUI
{
    class Application
    {
    public:
        static Core::NonNull<Application*> sharedApplication() { return &_sharedApplication; }

        void run();
        void quit();

    private:
        static Application _sharedApplication;

        Application() = default;
        ~Application() = default;

        Core::NonNull<Core::EventLoop*> _event_loop { Core::EventLoop::mainLoop() };
    };

    inline void Application::run()
    {
        _event_loop->run();
    }

    inline void Application::quit()
    {
        _event_loop->quit();
    }
}
