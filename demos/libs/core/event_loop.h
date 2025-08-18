#pragma once

#include <functional>
#include <poll.h>

namespace core {

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

    static auto main() -> event_loop& { return s_main_loop; }

    event_loop() = default;
    ~event_loop();

    auto add_source(source source) -> void;

    auto run() -> void;

private:
    static event_loop s_main_loop;

    std::vector<source> m_sources {};

    struct pollfd* m_poll_fds { nullptr };
};

}
