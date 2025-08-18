#pragma once

#include "window.h"
#include "wayland_display.h"

namespace gui {

class wayland_window : public window
{
public:
    explicit wayland_window(std::shared_ptr<wayland_display> display);
    ~wayland_window();

private:
    static const struct xdg_surface_listener s_xdg_surface_listener;
    static const struct wl_buffer_listener s_wl_buffer_listener;

    std::shared_ptr<wayland_display> m_display;

    struct wl_surface* m_wl_surface { nullptr };

    int m_shm_fd { -1 };
    struct wl_shm_pool* m_wl_shm_pool { nullptr };

    struct wl_buffer* m_wl_buffer { nullptr };

    struct xdg_surface* m_xdg_surface { nullptr };
    struct xdg_toplevel* m_xdg_toplevel { nullptr };
};

}
