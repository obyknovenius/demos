#include "event_loop.h"

#include <cstdlib>

namespace core
{
    event_loop event_loop::s_main = event_loop();

    event_loop::~event_loop()
    {
        if (m_poll_fds)
            free(m_poll_fds);
    }

    auto event_loop::add_source(source source) -> void
    {
        int n_poll_fds = m_sources.size();
        if (!m_poll_fds)
            m_poll_fds = static_cast<pollfd*>(malloc(sizeof(pollfd)));
        else
            m_poll_fds = static_cast<pollfd*>(realloc(m_poll_fds, (n_poll_fds + 1) * sizeof(pollfd)));
        ++n_poll_fds;

        m_poll_fds[n_poll_fds - 1].fd = source.fd;
        m_poll_fds[n_poll_fds - 1].events = source.events;

        m_sources.push_back(source);
    }

    auto event_loop::run() -> void
    {
        m_running = true;

        while (m_running)
        {
            for (const auto& source : m_sources)
                if (source.prepare)
                    source.prepare();

            int ret = poll(m_poll_fds, m_sources.size(), -1);
            if (ret > 0)
                for (const auto& source : m_sources)
                    if (m_poll_fds[0].fd == source.fd && m_poll_fds[0].revents & source.events)
                        source.dispatch();
        }
    }

    auto event_loop::quit() -> void
    {
        m_running = false;
    }
}
