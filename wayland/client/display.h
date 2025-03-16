#pragma once

#include <EGL/egl.h>
#include <wayland-client.h>
#include "xdg-shell-client-protocol.h"

struct display {
    struct wl_display *wl_display;
    struct wl_registry *wl_registry;
    struct wl_compositor *compositor;
    struct xdg_wm_base *xdg_wm_base;

    struct wl_registry_listener wl_registry_listener;
    struct xdg_wm_base_listener xdg_wm_base_listener;

    EGLDisplay egl_display;
};

struct display *
create_display();

void
destroy_display(struct display *display);
