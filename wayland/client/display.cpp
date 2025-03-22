#include "display.h"

#include <cstring>
#include <iostream>
#include <EGL/eglext.h>

static auto ping(void* data, struct xdg_wm_base* xdg_wm_base, uint32_t serial) -> void
{
    xdg_wm_base_pong(xdg_wm_base, serial);
}

struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = ping,
};

static auto registry_global(void* data, struct wl_registry* wl_registry, uint32_t name, const char* interface, uint32_t version) -> void
{
    auto display = reinterpret_cast<Display*>(data);
    display->registry_global(wl_registry, name, interface, version);
}

static auto registry_global_remove(void* data, struct wl_registry* registry, uint32_t name) -> void
{
    /* This space deliberately left blank */
}

struct wl_registry_listener wl_registry_listener = {
    .global = registry_global,
    .global_remove = registry_global_remove,
};

static auto print_egl_extensions(EGLDisplay display) -> void
{
    const char* extensions = eglQueryString(display, EGL_EXTENSIONS);
    std::cout << "EGL Extensions: " << extensions << std::endl;
}

Display::Display()
{
    m_wl_display = wl_display_connect(NULL);

    m_wl_registry = wl_display_get_registry(m_wl_display);
    wl_registry_add_listener(m_wl_registry, &wl_registry_listener, this);

    wl_display_roundtrip(m_wl_display);

    print_egl_extensions(EGL_NO_DISPLAY);

    m_egl_display = eglGetPlatformDisplay(EGL_PLATFORM_WAYLAND_KHR, m_wl_display, NULL);

    EGLint major, minor;
    eglInitialize(m_egl_display, &major, &minor);
    std::cout << "EGL " << major << "." << minor << std::endl;
    print_egl_extensions(m_egl_display);
}

Display::~Display()
{
    eglMakeCurrent(m_egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglTerminate(m_egl_display);

    wl_display_disconnect(m_wl_display);
}

auto Display::registry_global(struct wl_registry* wl_registry, uint32_t name, const char* interface, uint32_t version) -> void
{
    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        m_wl_compositor = reinterpret_cast<struct wl_compositor*>(wl_registry_bind(wl_registry, name, &wl_compositor_interface, 4));
    } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        m_xdg_wm_base = reinterpret_cast<struct xdg_wm_base*>(wl_registry_bind(wl_registry, name, &xdg_wm_base_interface, 1));
        xdg_wm_base_add_listener(m_xdg_wm_base, &xdg_wm_base_listener, this);
    }
}
