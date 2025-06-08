#include "server.h"

#include <cassert>
#include <cstring>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <iostream>
#include <wayland-client.h>
#include <xdg-shell-client.h>

#include "window.h"

namespace wayland {

#ifndef NDEBUG
static auto print_egl_extensions(EGLDisplay display) -> void
{
    const char* extensions = eglQueryString(display, EGL_EXTENSIONS);
    std::cout << "EGL Extensions: " << extensions << std::endl;
}
#endif

const struct wl_registry_listener Server::s_registry_listener = {
    .global = [](void* data, struct wl_registry* registry, uint32_t name, const char* interface, uint32_t) {
        auto* server = static_cast<Server*>(data);
        if (strcmp(interface, wl_compositor_interface.name) == 0) {
            server->m_compositor = reinterpret_cast<struct wl_compositor*>(wl_registry_bind(registry, name, &wl_compositor_interface, 4));
        } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
            server->m_xdg_wm_base = reinterpret_cast<struct xdg_wm_base*>(wl_registry_bind(registry, name, &xdg_wm_base_interface, 1));
            xdg_wm_base_add_listener(server->m_xdg_wm_base, &s_xdg_wm_base_listener, server);
        }
    }
};

const struct xdg_wm_base_listener Server::s_xdg_wm_base_listener = {
    .ping = [](void* data, struct xdg_wm_base*, uint32_t serial) {
        auto* server = static_cast<Server*>(data);
        xdg_wm_base_pong(server->m_xdg_wm_base, serial);
    }
};

Server::Server()
{
    m_display = wl_display_connect(nullptr);

    m_registry = wl_display_get_registry(m_display);
    wl_registry_add_listener(m_registry, &s_registry_listener, this);

    wl_display_roundtrip(m_display);

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

    wl_display_disconnect(m_display);
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
        if (wl_display_dispatch(m_display) < 0) {
            if (errno != EAGAIN) {
                perror("wl_display_dispatch");
                break;
            }
        }
    }
}

}
