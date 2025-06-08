#pragma once

#include <EGL/egl.h>
#include <gfx/size.h>
#include <wayland-client.h>
#include <wayland-egl.h>
#include <xdg-shell-client.h>

namespace wayland {

class Server;

class Window final {
public:
    Window(Server& server, const gfx::Size& size);
    ~Window();

    auto id() const -> int { return m_id; }

    auto draw() -> void;

private:
    int m_id;

    bool m_animate { false };

    gfx::Size m_size {};
    
    static const struct xdg_surface_listener s_xdg_surface_listener;

    struct wl_surface* m_surface {};
    struct xdg_surface* m_xdg_surface {};
    struct xdg_toplevel* m_xdg_toplevel {};

    struct wl_egl_window* m_egl_window {};

    EGLDisplay m_egl_display { EGL_NO_DISPLAY};
    EGLSurface m_egl_surface { EGL_NO_SURFACE };
    EGLContext m_egl_context { EGL_NO_CONTEXT };

    unsigned int m_program;
};

}
