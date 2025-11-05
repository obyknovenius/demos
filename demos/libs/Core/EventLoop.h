#pragma once

#include <functional>
#include <poll.h>

namespace Core
{
    class EventLoop
    {
    public:
        struct Source
        {
            int fd;
            short events;
            std::function<void()> prepare;
            std::function<void()> dispatch;
        };

        static EventLoop& mainLoop() { return _mainLoop; }

        EventLoop() = default;
        ~EventLoop();

        void addSource(Source source);

        void run();
        void quit();

    private:
        static EventLoop _mainLoop;

        bool _running { false };

        std::vector<Source> _sources {};

        pollfd* _poll_fds {};
    };
}
