#include "Display.h"

#include "Event.h"
#include "Seat.h"
#include "Window.h"
#include <Core/EventLoop.h>
#include <cstring>
#include <mutex>

namespace GUI::Wayland
{
    const wl_registry_listener Display::_wlRegistryListener = {
        .global = [](void* data, wl_registry* registry, uint32_t name, const char* interface, uint32_t version)
        {
            auto* display = reinterpret_cast<Display*>(data);
            display->onRegistryGlobal(registry, name, interface, version);
        },
        .global_remove = [](void *data, wl_registry *registry, uint32_t name)
        {
        }
    };

    const xdg_wm_base_listener Display::_xdgWmBaseListener = {
        .ping = [](void* data, xdg_wm_base* xdgWmBase, uint32_t serial)
        {
            auto* display = reinterpret_cast<Display*>(data);
            display->onWmPing(xdgWmBase, serial);
        }
    };

    RefPtr<Display> Display::_defaultDisplay { nullptr };

    RefPtr<Display> Display::defaultDisplay()
    {
        static std::once_flag once;
        std::call_once(once, []
        {
            if (!_defaultDisplay)
            {
                wl_display* wlDisplay = wl_display_connect(nullptr);
                if (!wlDisplay)
                    return;
                _defaultDisplay = adopt(*new Display(wlDisplay));
            }
        });
        return _defaultDisplay;
    }

    Display::Display(wl_display* wlDisplay) : _wlDisplay { wlDisplay }
    {
        _wlRegistry = wl_display_get_registry(_wlDisplay);
        wl_registry_add_listener(_wlRegistry, &_wlRegistryListener, this);
        wl_display_roundtrip(_wlDisplay);

        Core::EventLoop::mainLoop().addSource({
            .fd = wl_display_get_fd(_wlDisplay),
            .events = POLLIN,
            .prepare = [this]()
            {
                wl_display_flush(_wlDisplay);
            },
            .dispatch = [this]()
            {
                wl_display_dispatch(_wlDisplay);
            }
        });
    }

    Display::~Display()
    {
        _seat = nullptr;

        xdg_wm_base_destroy(_xdgWmBase);
        wl_compositor_destroy(_wlCompositor);
        wl_shm_destroy(_wlShm);

        wl_registry_destroy(_wlRegistry);

        wl_display_disconnect(_wlDisplay);
    }

    void Display::dispatchEvent(std::unique_ptr<const Event> event)
    {
        auto window = event->window;
        if (window)
            window->dispatchEvent(std::move(event));
    }

    void Display::onRegistryGlobal(wl_registry* registry, uint32_t name, const char* interface, uint32_t version)
    {
        if (strcmp(interface, wl_shm_interface.name) == 0)
        {
            _wlShm = reinterpret_cast<wl_shm*>(wl_registry_bind(registry, name, &wl_shm_interface, 1));
        }
        else if (strcmp(interface, wl_compositor_interface.name) == 0)
        {
            _wlCompositor = reinterpret_cast<wl_compositor*>(wl_registry_bind(registry, name, &wl_compositor_interface, 4));
        }
        else if (strcmp(interface, xdg_wm_base_interface.name) == 0)
        {
            _xdgWmBase = reinterpret_cast<xdg_wm_base*>(wl_registry_bind(registry, name, &xdg_wm_base_interface, 1));
            xdg_wm_base_add_listener(_xdgWmBase, &_xdgWmBaseListener, this);
        }
        else if (strcmp(interface, wl_seat_interface.name) == 0)
        {
            auto* wl_seat = reinterpret_cast<struct wl_seat*>(wl_registry_bind(registry, name, &wl_seat_interface, 7));
            _seat = Seat::make(wl_seat, *this);
        }
    }

    void Display::onWmPing(xdg_wm_base* xdgWmBase, uint32_t serial)
    {
        xdg_wm_base_pong(xdgWmBase, serial);
    }
}
