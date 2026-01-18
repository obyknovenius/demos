#include "Surface.h"

#include "Display.h"

namespace Gfx::Wayland
{
    const wl_surface_listener Surface::_wlSurfaceListener = {
        .enter = [](void* data, wl_surface* wlSurface, wl_output* wlOutput)
        {
            auto* surface = reinterpret_cast<Surface*>(data);
            surface->onSurfaceEnter(wlSurface, wlOutput);
        },
        .leave = [](void* data, wl_surface* wlSurface, wl_output* wlOutput)
        {
            auto* surface = reinterpret_cast<Surface*>(data);
            surface->onSurfaceLeave(wlSurface, wlOutput);
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
        _wlSurface = wl_compositor_create_surface(_display->globals().wlCompositor);
        wl_surface_add_listener(_wlSurface, &_wlSurfaceListener, this);

        _xdgSurface = xdg_wm_base_get_xdg_surface(_display->globals().xdgWmBase, _wlSurface);
        xdg_surface_add_listener(_xdgSurface, &_xdgSurfaceListener, this);

        _wlEglWindow = wl_egl_window_create(_wlSurface, _size.width, _size.height);

        EGLint attributes[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_NONE
        };
        EGLint configCount;
        eglChooseConfig(_display->eglDisplay(), attributes, &_eglConfig, 1, &configCount);

        _eglSurface = eglCreatePlatformWindowSurface(_display->eglDisplay(), _eglConfig, _wlEglWindow, NULL);
    }

    Surface::~Surface()
    {
        eglDestroySurface(_display->eglDisplay(), _eglSurface);

        wl_egl_window_destroy(_wlEglWindow);

        xdg_surface_destroy(_xdgSurface);
        wl_surface_destroy(_wlSurface);
    }

    void Surface::onSurfaceEnter(wl_surface* wlSurface, wl_output* wlOutput)
    {
    }

    void Surface::onSurfaceLeave(wl_surface* wlSurface, wl_output* wlOutput)
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
