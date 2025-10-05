#include "wayland_display.h"

#include "wayland_seat.h"
#include "wayland_window.h"
#include <core/event_loop.h>
#include <cstring>

namespace gui
{
    const wl_registry_listener wayland_display::s_wl_registry_listener = {
        .global = [](void* data, wl_registry* registry, uint32_t name, const char* interface, uint32_t version)
        {
            auto* display = reinterpret_cast<wayland_display*>(data);
            display->on_registry_global(registry, name, interface, version);
        },
        .global_remove = [](void *data, wl_registry *registry, uint32_t name)
        {
        }
    };

    const xdg_wm_base_listener wayland_display::s_xdg_wm_base_listener = {
        .ping = [](void* data, xdg_wm_base* xdg_wm_base, uint32_t serial)
        {
            auto* display = reinterpret_cast<wayland_display*>(data);
            display->on_wm_ping(xdg_wm_base, serial);
        }
    };

    ref_ptr<wayland_display> wayland_display::connect()
    {
        wl_display* wl_display = wl_display_connect(nullptr);
        if (!wl_display)
            return nullptr;
        return create(wl_display);
    }

    wayland_display::wayland_display(wl_display* wl_display) : m_wl_display { wl_display }
    {
        m_wl_registry = wl_display_get_registry(m_wl_display);
        wl_registry_add_listener(m_wl_registry, &s_wl_registry_listener, this);
        wl_display_roundtrip(m_wl_display);

        core::event_loop::get_main().add_source({
            .fd = wl_display_get_fd(m_wl_display),
            .events = POLLIN,
            .prepare = [this]()
            {
                wl_display_flush(m_wl_display);
            },
            .dispatch = [this]()
            {
                wl_display_dispatch(m_wl_display);
            }
        });
    }

    wayland_display::~wayland_display()
    {
        m_seat = nullptr;

        xdg_wm_base_destroy(m_xdg_wm_base);
        wl_compositor_destroy(m_wl_compositor);
        wl_shm_destroy(m_wl_shm);

        wl_registry_destroy(m_wl_registry);

        wl_display_disconnect(m_wl_display);
    }

    nonnull_ref_ptr<window> wayland_display::create_window()
    {
        return wayland_window::create(*this);
    }

    void wayland_display::on_registry_global(wl_registry* registry, uint32_t name, const char* interface, uint32_t version)
    {
        if (strcmp(interface, wl_shm_interface.name) == 0)
        {
            m_wl_shm = reinterpret_cast<wl_shm*>(wl_registry_bind(registry, name, &wl_shm_interface, 1));
        }
        else if (strcmp(interface, wl_compositor_interface.name) == 0)
        {
            m_wl_compositor = reinterpret_cast<wl_compositor*>(wl_registry_bind(registry, name, &wl_compositor_interface, 4));
        }
        else if (strcmp(interface, xdg_wm_base_interface.name) == 0)
        {
            m_xdg_wm_base = reinterpret_cast<xdg_wm_base*>(wl_registry_bind(registry, name, &xdg_wm_base_interface, 1));
            xdg_wm_base_add_listener(m_xdg_wm_base, &s_xdg_wm_base_listener, this);
        }
        else if (strcmp(interface, wl_seat_interface.name) == 0)
        {
            auto* wl_seat = reinterpret_cast<struct wl_seat*>(wl_registry_bind(registry, name, &wl_seat_interface, 7));
            m_seat = wayland_seat::create(wl_seat, *this);
        }
    }

    void wayland_display::on_wm_ping(xdg_wm_base* xdg_wm_base, uint32_t serial)
    {
        xdg_wm_base_pong(xdg_wm_base, serial);
    }
}
