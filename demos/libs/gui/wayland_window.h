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
    static const struct xdg_surface_listener s_xdg_surface_listener;
    static const struct wl_buffer_listener s_wl_buffer_listener;

    auto on_surface_configure(struct xdg_surface* xdg_surface, uint32_t serial) -> void;
    auto on_buffer_release(struct wl_buffer* buffer) -> void;

    wayland_display* m_display;

    struct wl_surface* m_wl_surface {};

    int m_shm_fd { -1 };
    struct wl_shm_pool* m_wl_shm_pool {};

    struct wl_buffer* m_wl_buffer {};

    struct xdg_surface* m_xdg_surface {};
    struct xdg_toplevel* m_xdg_toplevel {};

    bool m_closed { false };
};

}
