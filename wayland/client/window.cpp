
#include "window.h"

#include <cassert>
#include <chrono>
#include <iostream>
#include <optional>
#include <wayland-client.h>
#include <wayland-egl.h>
#include <GLES2/gl2.h>
#include "xdg-shell-client.h"

static auto print_fps() -> void
{
    using namespace std::chrono;

    static int frames = 0;
    static std::optional<high_resolution_clock::time_point> previous_time {};

    if (!previous_time) {
        previous_time = high_resolution_clock::now();
        return;
    }

    ++frames;

    auto current_time = high_resolution_clock::now();
    auto elapsed_time_ms = duration_cast<milliseconds>(current_time - *previous_time);
    if (elapsed_time_ms.count() >= 1000) {
        std::cout << "FPS: " << frames << std::endl;
        frames = 0;
        previous_time = current_time;
    }
}

static auto frame_done(void* data, struct wl_callback* callback, uint32_t time) -> void;

struct wl_callback_listener wl_surface_frame_listener = {
    .done = frame_done,
};

static auto frame_done(void* data, struct wl_callback* callback, uint32_t time) -> void
{
    auto window = reinterpret_cast<Window*>(data);
    window->frame_done(callback, time);
}

static auto configure(void* data, struct xdg_surface* xdg_surface, uint32_t serial) -> void
{
    auto window = reinterpret_cast<Window*>(data);
    window->configure(xdg_surface, serial);
}

struct xdg_surface_listener xdg_surface_listener = {
    .configure = configure,
};

Window::Window(Display& display, int width, int height)
{
    m_wl_surface = wl_compositor_create_surface(display.wl_compositor());

    m_xdg_surface = xdg_wm_base_get_xdg_surface(display.xdg_wm_base(), m_wl_surface);
    xdg_surface_add_listener(m_xdg_surface, &xdg_surface_listener, this);

    m_xdg_toplevel = xdg_surface_get_toplevel(m_xdg_surface);
    xdg_toplevel_set_title(m_xdg_toplevel, "Hello, Wayland!");
    wl_surface_commit(m_wl_surface);

    m_wl_egl_window = wl_egl_window_create(m_wl_surface, width, height);

    m_egl_display = display.egl_display();

    EGLint config_attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };
    EGLConfig egl_config;
    EGLint num_config;
    
    eglChooseConfig(m_egl_display, config_attribs, &egl_config, 1, &num_config);
    assert(num_config > 0);

    m_egl_surface = eglCreatePlatformWindowSurface(m_egl_display, egl_config, m_wl_egl_window, NULL);

    EGLint context_attribs[] = {
        EGL_CONTEXT_MAJOR_VERSION, 3,
        EGL_NONE
    };
    m_egl_context = eglCreateContext(m_egl_display, egl_config, EGL_NO_CONTEXT, context_attribs);

    eglMakeCurrent(m_egl_display, m_egl_surface, m_egl_surface, m_egl_context);
}

Window::~Window()
{
    if (eglGetCurrentContext() == m_egl_context) {
        eglMakeCurrent(m_egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    }
    eglDestroyContext(m_egl_display, m_egl_context);
    eglDestroySurface(m_egl_display, m_egl_surface);

    wl_egl_window_destroy(m_wl_egl_window);
    xdg_toplevel_destroy(m_xdg_toplevel);
    xdg_surface_destroy(m_xdg_surface);
    wl_surface_destroy(m_wl_surface);
}

auto Window::configure(struct xdg_surface* xdg_surface, uint32_t serial) -> void
{
    xdg_surface_ack_configure(xdg_surface, serial);

    struct wl_callback* callback = wl_surface_frame(m_wl_surface);
    wl_callback_add_listener(callback, &wl_surface_frame_listener, this);

    draw();
}

auto Window::draw() -> void
{
    static GLfloat color[4] = { 0.0, 1.0, 0.0, 1.0 };
    static int inc = 0, dec = 1;

    color[inc] += 0.01;
    color[dec] -= 0.01;

    if (color[dec] <= 0.0) {
        dec = inc;
        inc = (inc + 2) % 3;
    }

    glClearColor(color[0], color[1], color[2], color[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    glFlush();

    eglSwapBuffers(m_egl_display, m_egl_surface);

    print_fps();
}

auto Window::frame_done(struct wl_callback* callback, uint32_t time) -> void
{
	wl_callback_destroy(callback);
    callback = wl_surface_frame(m_wl_surface);
	wl_callback_add_listener(callback, &wl_surface_frame_listener, this);

    draw();
}
