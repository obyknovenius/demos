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

        static auto get_main() -> event_loop& { return s_main; }

        event_loop() = default;
        ~event_loop();

        auto add_source(source source) -> void;

        auto run() -> void;
        auto quit() -> void;

    private:
        static event_loop s_main;

        bool m_running { false };

        std::vector<source> m_sources {};

        pollfd* m_poll_fds {};
    };
}
