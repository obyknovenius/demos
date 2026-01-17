#include "Surface.h"

#include "Display.h"

namespace GUI::Wayland
{
    const wl_surface_listener Surface::_wlSurfaceListener = {
        .enter = [](void* data, wl_surface* wlSurface, wl_output* output)
        {
            auto* surface = reinterpret_cast<Surface*>(data);
            surface->onSurfaceEnter(wlSurface, output);
        },
        .leave = [](void* data, wl_surface* wlSurface, wl_output* output)
        {
            auto* surface = reinterpret_cast<Surface*>(data);
            surface->onSurfaceLeave(wlSurface, output);
        },
        .preferred_buffer_scale = [](void* data, wl_surface* wlSurface, int32_t scale)
        {
            auto* surface = reinterpret_cast<Surface*>(data);
            surface->onSurfacePreferredBufferScale(wlSurface, scale);
        },
        .preferred_buffer_transform = [](void* data, wl_surface* wlSurface, uint32_t transform)
        {
            auto* surface = reinterpret_cast<Surface*>(data);
            surface->onSurfacePreferredBufferTransform(wlSurface, transform);
        }
    };

    const xdg_surface_listener Surface::_xdgSurfaceListener = {
        .configure = [](void* data, xdg_surface* xdgSurface, uint32_t serial)
        {
            auto* surface = reinterpret_cast<Surface*>(data);
            surface->onSurfaceConfigure(xdgSurface, serial);
        }
    };

    Surface::Surface(NonNull<RefPtr<Display>> display) : _display { display }
    {
        _wlSurface = wl_compositor_create_surface(display->wlCompositor());

        _xdgSurface = xdg_wm_base_get_xdg_surface(display->xdgWmBase(), _wlSurface);

        _wlEglWindow = wl_egl_window_create(_wlSurface, _size.width, _size.height);

        _eglSurface = eglCreatePlatformWindowSurface(display->eglDisplay(), display->eglConfig(), _wlEglWindow, NULL);
    }

    Surface::~Surface()
    {
        if (auto display = _display.strong())
            eglDestroySurface(display->eglDisplay(), _eglSurface);

        wl_egl_window_destroy(_wlEglWindow);

        xdg_surface_destroy(_xdgSurface);

        wl_surface_destroy(_wlSurface);
    }

    void Surface::onSurfaceEnter(wl_surface* wlSurface, wl_output* output)
    {
    }

    void Surface::onSurfaceLeave(wl_surface* wlSurface, wl_output* output)
    {
    }

    void Surface::onSurfacePreferredBufferScale(wl_surface* wlSurface, int32_t scale)
    {
    }

    void Surface::onSurfacePreferredBufferTransform(wl_surface* wlSurface, uint32_t transform)
    {
    }

    void Surface::onSurfaceConfigure(xdg_surface* xdgSurface, uint32_t serial)
    {
        xdg_surface_ack_configure(xdgSurface, serial);
    }
}
