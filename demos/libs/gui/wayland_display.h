#pragma once

#include "display.h"

#include <wayland-client.h>
#include <xdg-shell-client-protocol.h>

namespace gui {

class wayland_display : public display, public std::enable_shared_from_this<wayland_display>
{
public:
    static auto connect() -> std::shared_ptr<wayland_display>;

    ~wayland_display();

    auto wl_display() -> struct wl_display* { return m_wl_display; }
    auto wl_compositor() -> struct wl_compositor* { return m_wl_compositor; }
    auto wl_shm() -> struct wl_shm* { return m_wl_shm; }
    auto xdg_wm_base() -> struct xdg_wm_base* { return m_xdg_wm_base; }

    auto create_window() -> std::shared_ptr<window> override;

private:
    explicit wayland_display(struct wl_display* wl_display);

    static const struct wl_registry_listener s_wl_registry_listener;
    static const struct xdg_wm_base_listener s_xdg_wm_base_listener;

    struct wl_display* m_wl_display { nullptr };
    struct wl_registry* m_wl_registry { nullptr };
    struct wl_shm* m_wl_shm { nullptr };
    struct wl_compositor* m_wl_compositor { nullptr };
    struct xdg_wm_base* m_xdg_wm_base { nullptr };
};

}
