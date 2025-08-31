#pragma once

#include "display.h"

#include "event.h"
#include <memory>
#include <wayland-client.h>
#include <xdg-shell-client-protocol.h>

namespace gui {

class wayland_seat;

class wayland_display : public display
{
public:
    static auto connect() -> std::shared_ptr<wayland_display>;

    ~wayland_display();

    auto create_window() -> std::shared_ptr<window> override;

private:
    explicit wayland_display(wl_display* wl_display);

    static const wl_registry_listener s_wl_registry_listener;
    static const xdg_wm_base_listener s_xdg_wm_base_listener;

    auto on_registry_global(wl_registry* registry, uint32_t name, const char* interface, uint32_t version) -> void;
    auto on_wm_ping(xdg_wm_base* xdg_wm_base, uint32_t serial) -> void;

    wl_display* m_wl_display {};
    wl_registry* m_wl_registry {};
    wl_shm* m_wl_shm {};
    wl_compositor* m_wl_compositor {};
    xdg_wm_base* m_xdg_wm_base {};
    wl_seat* m_wl_seat {};

    wayland_seat* m_seat {};

    friend class wayland_window;
};

}
