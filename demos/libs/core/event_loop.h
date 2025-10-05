#pragma once

#include <functional>
#include <poll.h>

namespace core
{
    class event_loop
    {
    public:
        struct source
        {
            int fd;
            short events;
            std::function<void()> prepare;
            std::function<void()> dispatch;
        };

        static event_loop& get_main() { return s_main; }

        event_loop() = default;
        ~event_loop();

        void add_source(source source);

        void run();
        void quit();

    private:
        static event_loop s_main;

        bool m_running { false };

        std::vector<source> m_sources {};

        pollfd* m_poll_fds {};
    };
}
