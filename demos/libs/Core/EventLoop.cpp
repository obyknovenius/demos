#include "EventLoop.h"

#include <cstdlib>

namespace Core
{
    EventLoop EventLoop::_main = EventLoop();

    EventLoop::~EventLoop()
    {
        if (_poll_fds)
            free(_poll_fds);
    }

    void EventLoop::addSource(Source source)
    {
        int n_poll_fds = _sources.size();
        if (!_poll_fds)
            _poll_fds = static_cast<pollfd*>(malloc(sizeof(pollfd)));
        else
            _poll_fds = static_cast<pollfd*>(realloc(_poll_fds, (n_poll_fds + 1) * sizeof(pollfd)));
        ++n_poll_fds;

        _poll_fds[n_poll_fds - 1].fd = source.fd;
        _poll_fds[n_poll_fds - 1].events = source.events;

        _sources.push_back(source);
    }

    void EventLoop::run()
    {
        _running = true;

        while (_running)
        {
            for (const auto& source : _sources)
                if (source.prepare)
                    source.prepare();

            int ret = poll(_poll_fds, _sources.size(), -1);
            if (ret > 0)
                for (const auto& source : _sources)
                    if (_poll_fds[0].fd == source.fd && _poll_fds[0].revents & source.events)
                        source.dispatch();
        }
    }

    void EventLoop::quit()
    {
        _running = false;
    }
}
