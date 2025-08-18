#include "wayland_display.h"

#include <core/event_loop.h>
#include <cstring>
#include <iostream>
#include "wayland_window.h"

namespace gui {

const struct wl_registry_listener wayland_display::s_wl_registry_listener = {
    .global = [](void* data, struct wl_registry* registry, uint32_t name, const char* interface, uint32_t version)
    {
        auto* display = reinterpret_cast<wayland_display*>(data);

        if (strcmp(interface, wl_shm_interface.name) == 0)
        {
            display->m_wl_shm = reinterpret_cast<struct wl_shm*>(wl_registry_bind(registry, name, &wl_shm_interface, 1));
        }
        else if (strcmp(interface, wl_compositor_interface.name) == 0)
        {
            display->m_wl_compositor = reinterpret_cast<struct wl_compositor*>(wl_registry_bind(registry, name, &wl_compositor_interface, 4));
        }
        else if (strcmp(interface, xdg_wm_base_interface.name) == 0)
        {
            display->m_xdg_wm_base = reinterpret_cast<struct xdg_wm_base*>(wl_registry_bind(registry, name, &xdg_wm_base_interface, 1));
            xdg_wm_base_add_listener(display->m_xdg_wm_base, &s_xdg_wm_base_listener, display);
        }
    },
    .global_remove = [](void *data, struct wl_registry *registry, uint32_t name)
    {
    }
};

const struct xdg_wm_base_listener wayland_display::s_xdg_wm_base_listener = {
    .ping = [](void* data, struct xdg_wm_base* xdg_wm_base, uint32_t serial)
    {
        xdg_wm_base_pong(xdg_wm_base, serial);
    }
};

auto wayland_display::connect() -> std::shared_ptr<wayland_display>
{
    struct wl_display* wl_display = wl_display_connect(nullptr);
    if (!wl_display)
        return nullptr;
    return std::shared_ptr<wayland_display>(new wayland_display(wl_display));
}

wayland_display::wayland_display(struct wl_display* wl_display) : m_wl_display(wl_display)
{
    m_wl_registry = wl_display_get_registry(m_wl_display);
    wl_registry_add_listener(m_wl_registry, &s_wl_registry_listener, this);
    wl_display_roundtrip(m_wl_display);

    core::event_loop::main().add_source({
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
    wl_display_disconnect(m_wl_display);
}

auto wayland_display::create_window() -> std::shared_ptr<window>
{
    return std::make_shared<wayland_window>(shared_from_this());
}

}
