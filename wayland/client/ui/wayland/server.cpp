#include "display_server.h"

#include <cassert>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <iostream>

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

    m_egl_display = eglGetPlatformDisplay(EGL_PLATFORM_WAYLAND_KHR, m_display, NULL);
    assert(m_egl_display != EGL_NO_DISPLAY);

    EGLint major, minor;
    eglInitialize(m_egl_display, &major, &minor);

#ifndef NDEBUG
    std::cout << "EGL " << major << "." << minor << std::endl;
    print_egl_extensions(m_egl_display);
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
    add_window(std::shared_ptr<Window>(window));
    return window->id();
}

auto Server::draw_window(int id) -> void
{
    auto window = find_window(id);
    if (window) {
        window->draw();
    } else {
        std::cerr << "Window with ID " << id << " not found." << std::endl;
    }
}

std::shared_ptr<Window> Server::find_window(int id) {
    auto it = m_windows.find(id);
    if (it != m_windows.end()) {
        return it->second;
    }
    return nullptr;
}

void Server::remove_window(int id) {
    auto it = m_windows.find(id);
    if (it != m_windows.end()) {
        m_windows.erase(it);
    }
}

void Server::add_window(std::shared_ptr<Window> window) {
    m_windows[window->id()] = window;
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
