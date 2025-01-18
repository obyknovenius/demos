
#include <stdlib.h>
#include <string.h>
#include <wayland-client.h>
#include <wayland-egl.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <unistd.h>
#include "xdg-shell-protocol.h"

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
    state->egl_window = wl_egl_window_create(state->wl_surface, 640, 480);
    state->egl_surface = eglCreateWindowSurface(state->egl_display, state->egl_conf, state->egl_window, NULL);

    eglMakeCurrent(state->egl_display, state->egl_surface, state->egl_surface, state->egl_context);
}

static void draw_blue(struct  client_state *state)
{
    glClearColor(0.0, 0.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    eglSwapBuffers(state->egl_display, state->egl_surface);
}

static void draw_red(struct  client_state *state)
{
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    eglSwapBuffers(state->egl_display, state->egl_surface);
}

int
main(int argc, char *arg[])
{
    struct client_state state = { 0 };

    state.wl_display = wl_display_connect(NULL);
    state.wl_registry = wl_display_get_registry(state.wl_display);
    wl_registry_add_listener(state.wl_registry, &wl_registry_listener, &state);
    wl_display_roundtrip(state.wl_display);

    state.wl_surface = wl_compositor_create_surface(state.compositor);
    state.xdg_surface = xdg_wm_base_get_xdg_surface(state.xdg_wm_base, state.wl_surface);
    state.xdg_toplevel = xdg_surface_get_toplevel(state.xdg_surface);
    xdg_toplevel_set_title(state.xdg_toplevel, "Hello, Wayland!");

    init_egl(&state);
    create_window(&state);
    draw_blue(&state);

    sleep(2);

    draw_red(&state);

    while (wl_display_dispatch(state.wl_display)) {
        /* This space deliberately left blank */
    }
    
    return 0;
}