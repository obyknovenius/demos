#pragma once

#include <EGL/egl.h>
#include <wayland-client.h>

#include "ui/display_server.h"
#include "xdg-shell-client.h"

namespace ui::wayland {

class DisplayServer final : public ui::DisplayServer {
public:
    DisplayServer();
    ~DisplayServer();

    auto wl_display() const -> struct wl_display* { return m_wl_display; }
    auto wl_registry() const -> struct wl_registry* { return m_wl_registry; }
    auto wl_compositor() const -> struct wl_compositor* { return m_wl_compositor; }
    auto xdg_wm_base() const -> struct xdg_wm_base* { return m_xdg_wm_base; }

    auto egl_display() const -> EGLDisplay { return m_egl_display; }

    auto registry_global(struct wl_registry* wl_registry, uint32_t name, const char* interface, uint32_t version) -> void;

    auto create_window(gfx::Rect frame) -> int override;

    auto run() -> void override;

private:
    struct wl_display* m_wl_display {};
    struct wl_registry* m_wl_registry {};
    struct wl_compositor* m_wl_compositor {};
    struct xdg_wm_base* m_xdg_wm_base {};

    EGLDisplay m_egl_display { EGL_NO_DISPLAY };

    int m_last_window_id { 0 };
};

}
