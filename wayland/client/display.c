#include "display.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <EGL/eglext.h>

static void
xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial)
{
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static void
registry_global(void *data, struct wl_registry *wl_registry, uint32_t name,
                const char *interface, uint32_t version)
{
    struct display *display = data;
    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        display->compositor = wl_registry_bind(wl_registry, name, &wl_compositor_interface, 4);
    } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        display->xdg_wm_base = wl_registry_bind(wl_registry, name, &xdg_wm_base_interface, 1);
        display->xdg_wm_base_listener.ping = xdg_wm_base_ping;
        xdg_wm_base_add_listener(display->xdg_wm_base, &display->xdg_wm_base_listener, display);
    }
}

static void
registry_global_remove(void *data, struct wl_registry *registry, uint32_t name)
{
    /* This space deliberately left blank */
}

static void
print_egl_extensions(EGLDisplay display)
{
    const char *extensions = eglQueryString(display, EGL_EXTENSIONS);
    printf("EGL Extensions: %s\n", extensions);
}

struct display *
create_display()
{
    struct display *display;

    EGLint major, minor;
    
    display = malloc(sizeof(struct display));
    memset(display, 0, sizeof(struct display));

    display->wl_display = wl_display_connect(NULL);

    display->wl_registry = wl_display_get_registry(display->wl_display);
    display->wl_registry_listener.global = registry_global;
    display->wl_registry_listener.global_remove = registry_global_remove;
    wl_registry_add_listener(display->wl_registry, &display->wl_registry_listener, display);

    wl_display_roundtrip(display->wl_display);

    print_egl_extensions(EGL_NO_DISPLAY);

    display->egl_display = eglGetPlatformDisplay(EGL_PLATFORM_WAYLAND_KHR, display->wl_display, NULL);

    eglInitialize(display->egl_display, &major, &minor);
    printf("EGL %d.%d\n", major, minor);
    print_egl_extensions(display->egl_display);

    return display;
}

void
destroy_display(struct display *display)
{
    eglMakeCurrent(display->egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglTerminate(display->egl_display);

    wl_display_disconnect(display->wl_display);

    free(display);
}
