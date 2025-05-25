#include "display_server.h"

#include <cstring>
#include <iostream>
#include <EGL/eglext.h>

#include "window.h"

namespace ui::wayland {

static auto ping(void* data, struct xdg_wm_base* xdg_wm_base, uint32_t serial) -> void
{
    xdg_wm_base_pong(xdg_wm_base, serial);
}

struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = ping,
};

static auto registry_global(void* data, struct wl_registry* wl_registry, uint32_t name, const char* interface, uint32_t version) -> void
{
    auto display = reinterpret_cast<DisplayServer*>(data);
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

const char* vertex_shader_source = R"(
#version 300 es

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 transform;
uniform mat4 projection;

void main()
{
    gl_Position = projection * transform * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)";

const char* fragment_shader_source = R"(
#version 300 es
precision mediump float;

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoord);
}
)";

DisplayServer::DisplayServer()
{
    m_wl_display = wl_display_connect(NULL);

    m_wl_registry = wl_display_get_registry(m_wl_display);
    wl_registry_add_listener(m_wl_registry, &wl_registry_listener, this);

    wl_display_roundtrip(m_wl_display);

#ifndef NDEBUG
    print_egl_extensions(EGL_NO_DISPLAY);
#endif

    m_egl_display = eglGetPlatformDisplay(EGL_PLATFORM_WAYLAND_KHR, m_wl_display, NULL);

    EGLint major, minor;
    eglInitialize(m_egl_display, &major, &minor);

#ifndef NDEBUG
    std::cout << "EGL " << major << "." << minor << std::endl;
    print_egl_extensions(m_egl_display);
#endif
}

DisplayServer::~DisplayServer()
{
    eglMakeCurrent(m_egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglTerminate(m_egl_display);

    wl_display_disconnect(m_wl_display);
}

auto DisplayServer::registry_global(struct wl_registry* wl_registry, uint32_t name, const char* interface, uint32_t version) -> void
{
    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        m_wl_compositor = reinterpret_cast<struct wl_compositor*>(wl_registry_bind(wl_registry, name, &wl_compositor_interface, 4));
    } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        m_xdg_wm_base = reinterpret_cast<struct xdg_wm_base*>(wl_registry_bind(wl_registry, name, &xdg_wm_base_interface, 1));
        xdg_wm_base_add_listener(m_xdg_wm_base, &xdg_wm_base_listener, this);
    }
}

auto DisplayServer::create_window(gfx::Rect frame) -> int
{
    auto last_window_id = m_last_window_id++;
    auto* window = new Window { *this, last_window_id, frame };
    return last_window_id;
}

auto DisplayServer::run() -> void
{
    while (true) {
        if (wl_display_dispatch(m_wl_display) < 0) {
            if (errno != EAGAIN) {
                perror("wl_display_dispatch");
                break;
            }
        }
    }
}

}
