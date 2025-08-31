#pragma once

#include "window.h"
#include "wayland_display.h"
#include <memory>

namespace gui {

class wayland_window : public window, public std::enable_shared_from_this<wayland_window>
{
public:
    explicit wayland_window(wayland_display* display);
    ~wayland_window();

    auto close() -> void override;

private:
    static const xdg_surface_listener s_xdg_surface_listener;
    static const wl_buffer_listener s_wl_buffer_listener;

    auto on_surface_configure(xdg_surface* xdg_surface, uint32_t serial) -> void;
    auto on_buffer_release(wl_buffer* buffer) -> void;

    wayland_display* m_display;

    wl_surface* m_wl_surface {};

    int m_shm_fd { -1 };
    wl_shm_pool* m_wl_shm_pool {};

    wl_buffer* m_wl_buffer {};

    xdg_surface* m_xdg_surface {};
    xdg_toplevel* m_xdg_toplevel {};

    bool m_closed { false };
};

}
