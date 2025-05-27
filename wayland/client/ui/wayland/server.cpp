#include "display_server.h"

#include <iostream>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "window.h"

namespace wayland {

#ifndef NDEBUG
static auto print_egl_extensions(EGLDisplay display) -> void
{
    const char* extensions = eglQueryString(display, EGL_EXTENSIONS);
    std::cout << "EGL Extensions: " << extensions << std::endl;
}
#endif

Server::Server()
{
    m_registry = m_display.get_registry();
    m_registry.on_global() = [this](uint32_t name, const std::string& interface, uint32_t version) {
        if (interface == compositor_t::interface_name)
            m_registry.bind(name, m_compositor, version);
        else if (interface == xdg_wm_base_t::interface_name)
            m_registry.bind(name, m_xdg_wm_base, version);
    };
    m_display.roundtrip();

#ifndef NDEBUG
    print_egl_extensions(EGL_NO_DISPLAY);
#endif

    EGLDisplay egl_display = eglGetPlatformDisplay(EGL_PLATFORM_WAYLAND_KHR, m_display, NULL);

    EGLint major, minor;
    eglInitialize(egl_display, &major, &minor);

#ifndef NDEBUG
    std::cout << "EGL " << major << "." << minor << std::endl;
    print_egl_extensions(egl_display);
#endif
}

Server::~Server()
{
    eglMakeCurrent(m_egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglTerminate(m_egl_display);
}

auto Server::create_window(gfx::Size size) -> int
{
    auto* window = new Window { *this, size };
    return window->id();
}

auto Server::run() -> void
{
    while (true) {
        if (m_display.dispatch() < 0) {
            if (errno != EAGAIN) {
                perror("wl_display_dispatch");
                break;
            }
        }
    }
}

}
