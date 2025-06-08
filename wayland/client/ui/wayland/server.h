#pragma once

#include <wayland-client.h>
#include <xdg-shell-client.h>
#include <EGL/egl.h>

#include <map>
#include <memory>

#include "../display_server.h"

namespace wayland {

class Window;

class Server final : public ui::DisplayServer {
public:
    Server();
    ~Server();

    auto display() -> struct wl_display* { return m_display; }
    auto compositor() -> struct wl_compositor* { return m_compositor; }
    auto xdg_wm_base() -> struct xdg_wm_base* { return m_xdg_wm_base; }

    auto egl_display() -> EGLDisplay { return m_egl_display; }

    auto create_window(gfx::Size size) -> int override;

    auto draw_window(int id) -> void override;

    auto run() -> void override;

    auto next_window_id() -> int { return m_last_window_id++; }

private:
    static const struct wl_registry_listener s_registry_listener;
    static const struct xdg_wm_base_listener s_xdg_wm_base_listener;

    struct wl_display* m_display {};
    struct wl_registry* m_registry {};
    struct wl_compositor* m_compositor {};
    struct xdg_wm_base* m_xdg_wm_base {};

    EGLDisplay m_egl_display { EGL_NO_DISPLAY };

    int m_last_window_id { 0 };

    std::map<int, std::shared_ptr<Window>> m_windows {};

    std::shared_ptr<Window> find_window(int id);
    void remove_window(int id);
    void add_window(std::shared_ptr<Window> window);
};

}
