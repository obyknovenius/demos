
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wayland-client.h>
#include <wayland-egl.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <unistd.h>
#include "xdg-shell-client-protocol.h"

struct client_state {
    /* Globals */
    struct wl_display *wl_display;
    struct wl_registry *wl_registry;
    struct wl_compositor *compositor;
    struct xdg_wm_base *xdg_wm_base;

    /* Objects */
    struct wl_surface *wl_surface;
    struct xdg_surface *xdg_surface;
    struct xdg_toplevel *xdg_toplevel;

    /* EGL */
    struct wl_egl_window *egl_window;
    EGLDisplay egl_display;
    EGLConfig egl_conf;
    EGLSurface egl_surface;
    EGLContext egl_context;

    int width;
    int height;
};

static const struct wl_callback_listener wl_surface_frame_listener;

static void draw(struct client_state *state);

static void
xdg_surface_configure(void *data, struct xdg_surface *xdg_surface, uint32_t serial)
{
    struct client_state *state = data;

    xdg_surface_ack_configure(xdg_surface, serial);

    struct wl_callback *callback = wl_surface_frame(state->wl_surface);
    wl_callback_add_listener(callback, &wl_surface_frame_listener, state);

    draw(state);
}

static const struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_configure,
};

static void
xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial)
{
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = xdg_wm_base_ping,
};

static void
wl_surface_frame_done(void *data, struct wl_callback *callback, uint32_t time)
{
    struct client_state *state = data;

	wl_callback_destroy(callback);
    callback = wl_surface_frame(state->wl_surface);
	wl_callback_add_listener(callback, &wl_surface_frame_listener, state);

    draw(state);
}

static const struct wl_callback_listener wl_surface_frame_listener = {
	.done = wl_surface_frame_done,
};

static void
registry_global(void *data, struct wl_registry *wl_registry, uint32_t name,
                const char *interface, uint32_t version)
{
    struct client_state *state = data;
    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        state->compositor = wl_registry_bind(wl_registry, name, &wl_compositor_interface, 4);
    } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        state->xdg_wm_base = wl_registry_bind(wl_registry, name, &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(state->xdg_wm_base, &xdg_wm_base_listener, state);
    }
}

static void
registry_global_remove(void *data, struct wl_registry *registry, uint32_t name)
{
    /* This space deliberately left blank */
}

static const struct wl_registry_listener wl_registry_listener = {
    .global = registry_global,
    .global_remove = registry_global_remove,
};

static void
init_egl(struct client_state *state)
{
    EGLint major, minor, count, n, size;
    EGLConfig *configs;
    int i;
    EGLint config_attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };

    static const EGLint context_attribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    state->egl_display = eglGetDisplay((EGLNativeDisplayType)state->wl_display);

    eglInitialize(state->egl_display, &major, &minor);

    eglGetConfigs(state->egl_display, NULL, 0, &count);
    configs = calloc(count, sizeof *configs);
    eglChooseConfig(state->egl_display, config_attribs, configs, count, &n);

    for (i = 0; i < n; ++i) {
        eglGetConfigAttrib(state->egl_display, configs[i], EGL_BUFFER_SIZE, &size);
        eglGetConfigAttrib(state->egl_display, configs[i], EGL_RED_SIZE, &size);
        state->egl_conf = configs[i];
        break;
    }

    state->egl_context = eglCreateContext(state->egl_display, state->egl_conf, EGL_NO_CONTEXT, context_attribs);
}

static void
create_window(struct client_state *state)
{
    state->egl_window = wl_egl_window_create(state->wl_surface, state->width, state->height);
    state->egl_surface = eglCreateWindowSurface(state->egl_display, state->egl_conf, state->egl_window, NULL);

    eglMakeCurrent(state->egl_display, state->egl_surface, state->egl_surface, state->egl_context);
}

static void print_fps()
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

static void draw(struct client_state *state)
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
    eglSwapBuffers(state->egl_display, state->egl_surface);

    print_fps();
}

int
main(int argc, char *argv[])
{
    struct client_state state = { 0 , .width = 640, .height = 480 };

    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "-w") == 0)
            state.width = atoi(argv[i + 1]);
        else if (strcmp(argv[i], "-h") == 0)
            state.height = atoi(argv[i + 1]);
    }

    state.wl_display = wl_display_connect(NULL);
    state.wl_registry = wl_display_get_registry(state.wl_display);
    wl_registry_add_listener(state.wl_registry, &wl_registry_listener, &state);
    wl_display_roundtrip(state.wl_display);

    state.wl_surface = wl_compositor_create_surface(state.compositor);
    state.xdg_surface = xdg_wm_base_get_xdg_surface(state.xdg_wm_base, state.wl_surface);
    xdg_surface_add_listener(state.xdg_surface, &xdg_surface_listener, &state);
    state.xdg_toplevel = xdg_surface_get_toplevel(state.xdg_surface);
    xdg_toplevel_set_title(state.xdg_toplevel, "Hello, Wayland!");
    wl_surface_commit(state.wl_surface);

    init_egl(&state);
    create_window(&state);

    while (wl_display_dispatch(state.wl_display)) {
        /* This space deliberately left blank */
    }

    return 0;
}
