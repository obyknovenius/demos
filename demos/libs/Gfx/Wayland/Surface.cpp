#include "Surface.h"

#include "Display.h"
#include <GLES3/gl3.h>

namespace Gfx::Wayland
{
    const wl_surface_listener Surface::_wlSurfaceListener = {
        .enter = [](void* data, wl_surface* wlSurface, wl_output* wlOutput)
        {
            auto* surface = reinterpret_cast<Surface*>(data);
            surface->onWlSurfaceEnter(wlSurface, wlOutput);
        },
        .leave = [](void* data, wl_surface* wlSurface, wl_output* wlOutput)
        {
            auto* surface = reinterpret_cast<Surface*>(data);
            surface->onWlSurfaceLeave(wlSurface, wlOutput);
        },
        .preferred_buffer_scale = [](void* data, wl_surface* wlSurface, int32_t scale)
        {
            auto* surface = reinterpret_cast<Surface*>(data);
            surface->onWlSurfacePreferredBufferScale(wlSurface, scale);
        },
        .preferred_buffer_transform = [](void* data, wl_surface* wlSurface, uint32_t transform)
        {
            auto* surface = reinterpret_cast<Surface*>(data);
            surface->onWlSurfacePreferredBufferTransform(wlSurface, transform);
        }
    };

    const wl_callback_listener Surface::_wlFrameCallbackListener = {
        .done = [](void* data, wl_callback* wlCallback, uint32_t time)
        {
            auto* surface = reinterpret_cast<Surface*>(data);
            surface->didFinishFrame();
        }
    };

    const xdg_surface_listener Surface::_xdgSurfaceListener = {
        .configure = [](void* data, xdg_surface* xdgSurface, uint32_t serial)
        {
            auto* surface = reinterpret_cast<Surface*>(data);
            surface->onXdgSurfaceConfigure(xdgSurface, serial);
        }
    };

    Surface::Surface(NonNull<RefPtr<Display>> display) : _display { display }
    {
        _wlSurface = wl_compositor_create_surface(_display->globals().wlCompositor);
        wl_surface_set_user_data(_wlSurface, this);
        wl_surface_add_listener(_wlSurface, &_wlSurfaceListener, this);

        _xdgSurface = xdg_wm_base_get_xdg_surface(_display->globals().xdgWmBase, _wlSurface);
        xdg_surface_add_listener(_xdgSurface, &_xdgSurfaceListener, this);

        _wlEglWindow = wl_egl_window_create(_wlSurface, _size.width, _size.height);

        EGLint configAttributes[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_NONE
        };
        EGLint configCount;
        eglChooseConfig(_display->eglDisplay(), configAttributes, &_eglConfig, 1, &configCount);

        _eglSurface = eglCreatePlatformWindowSurface(_display->eglDisplay(), _eglConfig, _wlEglWindow, NULL);

        EGLint contextAttributes[] = {
            EGL_CONTEXT_MAJOR_VERSION, 3,
            EGL_NONE
        };

        _eglContext = eglCreateContext(_display->eglDisplay(), _eglConfig, EGL_NO_CONTEXT, contextAttributes);
    }

    Surface::~Surface()
    {
        eglDestroyContext(_display->eglDisplay(), _eglContext);
        eglDestroySurface(_display->eglDisplay(), _eglSurface);

        wl_egl_window_destroy(_wlEglWindow);

        xdg_surface_destroy(_xdgSurface);
        wl_surface_destroy(_wlSurface);
    }

    void Surface::beginFrame()
    {
        eglMakeCurrent(_display->eglDisplay(), _eglSurface, _eglSurface, _eglContext);
        eglSwapInterval(_display->eglDisplay(), 0);

        glViewport(0, 0, _size.width, _size.height);
    }

    void Surface::endFrame()
    {
        _wlFrameCallback = wl_surface_frame(_wlSurface);
        wl_callback_add_listener(_wlFrameCallback, &_wlFrameCallbackListener, this);

        eglSwapBuffers(_display->eglDisplay(), _eglSurface);
    }

    void Surface::didFinishFrame()
    {
        wl_callback_destroy(_wlFrameCallback);
        _wlFrameCallback = nullptr;

        if (auto delegate = _delegate.strong()) {
            delegate->surfaceDidFinishFrame(this);
        }
    }

    void Surface::onWlSurfaceEnter(wl_surface* wlSurface, wl_output* wlOutput)
    {
    }

    void Surface::onWlSurfaceLeave(wl_surface* wlSurface, wl_output* wlOutput)
    {
    }

    void Surface::onWlSurfacePreferredBufferScale(wl_surface* wlSurface, int32_t scale)
    {
    }

    void Surface::onWlSurfacePreferredBufferTransform(wl_surface* wlSurface, uint32_t transform)
    {
    }

    void Surface::onXdgSurfaceConfigure(xdg_surface* xdgSurface, uint32_t serial)
    {
        xdg_surface_ack_configure(xdgSurface, serial);
    }
}
