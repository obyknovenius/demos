#pragma once

#include "display.h"

struct window {
    struct wl_surface *wl_surface;
    struct xdg_surface *xdg_surface;
    struct xdg_toplevel *xdg_toplevel;

    struct xdg_surface_listener xdg_surface_listener;
    struct wl_callback_listener wl_surface_frame_listener;

    struct wl_egl_window *wl_egl_window;

    EGLDisplay egl_display;
    EGLSurface egl_surface;
    EGLContext egl_context;
};

struct window *
create_window(struct display *display, int width, int height);

void
destroy_window(struct window *window);
