#pragma once

#include <wayland-client.hpp>
#include <xdg-shell-client.hpp>
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

    auto display() -> display_t& { return m_display; }
    auto compositor() -> compositor_t& { return m_compositor; }
    auto xdg_wm_base() -> xdg_wm_base_t& { return m_xdg_wm_base; }

    auto egl_display() -> EGLDisplay { return m_egl_display; }

    auto create_window(gfx::Size size) -> int override;

    auto draw_window(int id) -> void override;

    auto run() -> void override;

    auto next_window_id() -> int { return m_last_window_id++; }

private:
    display_t m_display {};
    registry_t m_registry {};
    compositor_t m_compositor {};
    xdg_wm_base_t m_xdg_wm_base {};

    EGLDisplay m_egl_display { EGL_NO_DISPLAY };

    int m_last_window_id { 0 };

    std::map<int, std::shared_ptr<Window>> m_windows {};

    std::shared_ptr<Window> find_window(int id);
    void remove_window(int id);
    void add_window(std::shared_ptr<Window> window);
};

}
