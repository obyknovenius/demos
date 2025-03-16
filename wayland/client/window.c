
#include "window.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wayland-client.h>
#include <wayland-egl.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "xdg-shell-client-protocol.h"

static void
print_fps()
{
    static int frames = 0;
    static struct timespec previous_time = { 0 }, current_time;
    long elapsed_time;

    if (previous_time.tv_sec == 0) {
        clock_gettime(CLOCK_MONOTONIC, &previous_time);
        return;
    }

    ++frames;
    clock_gettime(CLOCK_MONOTONIC, &current_time);

    elapsed_time = (current_time.tv_sec - previous_time.tv_sec) * 1e9 + (current_time.tv_nsec - previous_time.tv_nsec);

    if (elapsed_time >= 1e9) {
        printf("FPS: %d\n", frames);
        frames = 0;
        previous_time = current_time;
    }
}

static void
draw_window(struct window *window)
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
    eglSwapBuffers(window->egl_display, window->egl_surface);

    print_fps();
}

static void
wl_surface_frame_done(void *data, struct wl_callback *callback, uint32_t time)
{
    struct window *window = data;

	wl_callback_destroy(callback);
    callback = wl_surface_frame(window->wl_surface);
    window->wl_surface_frame_listener.done = wl_surface_frame_done;
	wl_callback_add_listener(callback, &window->wl_surface_frame_listener, window);

    draw_window(window);
}

static void
xdg_surface_configure(void *data, struct xdg_surface *xdg_surface, uint32_t serial)
{
    struct window *window = data;

    xdg_surface_ack_configure(xdg_surface, serial);

    struct wl_callback *callback = wl_surface_frame(window->wl_surface);
    window->wl_surface_frame_listener.done = wl_surface_frame_done;
    wl_callback_add_listener(callback, &window->wl_surface_frame_listener, window);

    draw_window(window);
}

struct window *
create_window(struct display *display, int width, int height)
{
    struct window *window;

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
    EGLint context_attribs[] = {
        EGL_CONTEXT_MAJOR_VERSION, 3,
        EGL_NONE
    };

    window = malloc(sizeof(struct window));
    memset(window, 0, sizeof(struct window));

    window->wl_surface = wl_compositor_create_surface(display->compositor);

    window->xdg_surface = xdg_wm_base_get_xdg_surface(display->xdg_wm_base, window->wl_surface);
    window->xdg_surface_listener.configure = xdg_surface_configure;
    xdg_surface_add_listener(window->xdg_surface, &window->xdg_surface_listener, window);

    window->xdg_toplevel = xdg_surface_get_toplevel(window->xdg_surface);
    xdg_toplevel_set_title(window->xdg_toplevel, "Hello, Wayland!");
    wl_surface_commit(window->wl_surface);

    window->wl_egl_window = wl_egl_window_create(window->wl_surface, width, height);

    window->egl_display = display->egl_display;

    eglChooseConfig(window->egl_display, config_attribs, &egl_config, 1, &num_config);
    assert(num_config > 0);

    window->egl_surface = eglCreatePlatformWindowSurface(window->egl_display, egl_config, window->wl_egl_window, NULL);
    window->egl_context = eglCreateContext(window->egl_display, egl_config, EGL_NO_CONTEXT, context_attribs);

    eglMakeCurrent(window->egl_display, window->egl_surface, window->egl_surface, window->egl_context);

    return window;
}

void
destroy_window(struct window *window)
{
    if (eglGetCurrentContext() == window->egl_context) {
        eglMakeCurrent(window->egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    }
    eglDestroyContext(window->egl_display, window->egl_context);
    eglDestroySurface(window->egl_display, window->egl_surface);

    wl_egl_window_destroy(window->wl_egl_window);
    xdg_toplevel_destroy(window->xdg_toplevel);
    xdg_surface_destroy(window->xdg_surface);
    wl_surface_destroy(window->wl_surface);

    free(window);
}
