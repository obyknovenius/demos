#pragma once

#include "window.h"
#include <core/weak_ptr.h>
#include <wayland-client.h>
#include <xdg-shell-client-protocol.h>

namespace gui {

class wayland_display;

class wayland_window final : public window
{
    template<typename T, class... Args>
    friend nonnull_ref_ptr<T> core::make_ref_counted(Args&&...);

public:
    auto close() -> void override;

private:
    static const xdg_surface_listener s_xdg_surface_listener;
    static const wl_buffer_listener s_wl_buffer_listener;

    explicit wayland_window(nonnull_ref_ptr<wayland_display> display);
    ~wayland_window();

    auto on_surface_configure(xdg_surface* xdg_surface, uint32_t serial) -> void;
    auto on_buffer_release(wl_buffer* buffer) -> void;

    weak_ptr<wayland_display> m_display;

    wl_surface* m_wl_surface {};

    int m_shm_fd { -1 };
    wl_shm_pool* m_wl_shm_pool {};

    wl_buffer* m_wl_buffer {};

    xdg_surface* m_xdg_surface {};
    xdg_toplevel* m_xdg_toplevel {};

    bool m_closed { false };
};

}
