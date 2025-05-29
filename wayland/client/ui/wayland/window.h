#pragma once

#include <wayland-egl.hpp>
#include <gfx/size.h>

#include "server.h"

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

    surface_t m_surface {};
    xdg_surface_t m_xdg_surface {};
    xdg_toplevel_t m_xdg_toplevel {};

    egl_window_t m_egl_window {};

    EGLDisplay m_egl_display { EGL_NO_DISPLAY};
    EGLSurface m_egl_surface { EGL_NO_SURFACE };
    EGLContext m_egl_context { EGL_NO_CONTEXT };

    unsigned int m_program;
};

}
