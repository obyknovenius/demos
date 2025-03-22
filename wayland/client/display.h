#pragma once

#include <EGL/egl.h>
#include <wayland-client.h>
#include "xdg-shell-client.h"

class Display final {
public:
    Display();
    ~Display();

    auto wl_display() -> struct wl_display* { return m_wl_display; }
    auto wl_compositor() -> struct wl_compositor* { return m_wl_compositor; }
    auto xdg_wm_base() -> struct xdg_wm_base* { return m_xdg_wm_base; }

    auto egl_display() -> EGLDisplay { return m_egl_display; }

    auto registry_global(struct wl_registry* wl_registry, uint32_t name, const char* interface, uint32_t version) -> void;

private:
    struct wl_display* m_wl_display {};
    struct wl_registry* m_wl_registry {};
    struct wl_compositor* m_wl_compositor {};
    struct xdg_wm_base* m_xdg_wm_base {};

    EGLDisplay m_egl_display { EGL_NO_DISPLAY };
};
