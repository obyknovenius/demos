#pragma once

#include <EGL/egl.h>

#include "display.h"

class Window final {
public:
    Window(Display& display, int width, int height);
    ~Window();

    auto configure(struct xdg_surface* xdg_surface, uint32_t serial) -> void;
    auto frame_done(struct wl_callback* callback, uint32_t time) -> void;

private:
    auto draw() -> void;

    struct wl_surface* m_wl_surface {};
    struct xdg_surface* m_xdg_surface {};
    struct xdg_toplevel* m_xdg_toplevel {};

    struct wl_egl_window* m_wl_egl_window {};

    EGLDisplay m_egl_display { EGL_NO_DISPLAY};
    EGLSurface m_egl_surface { EGL_NO_SURFACE };
    EGLContext m_egl_context { EGL_NO_CONTEXT };
};
