#include "Surface.h"

#include "Display.h"
#include <GLES3/gl3.h>

namespace Gfx::Wayland
{
    const wl_callback_listener Surface::_frameCallbackListener = {
        .done = [](void* data, wl_callback* wlCallback, uint32_t time)
        {
            auto* surface = reinterpret_cast<Surface*>(data);
            surface->frameDidFinish(time);
        }
    };

    const xdg_surface_listener Surface::_xdgSurfaceListener = {
        .configure = [](void* data, xdg_surface* xdgSurface, uint32_t serial)
        {
            auto* surface = reinterpret_cast<Surface*>(data);
            surface->didConfigure(serial);
        }
    };

    Surface::Surface(NonNull<RefPtr<Display>> display) : _display { display }
    {
        _wlSurface = wl_compositor_create_surface(_display->globals().wlCompositor);
        wl_surface_set_user_data(_wlSurface, this);

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
        wl_callback_destroy(_frameCallback);

        eglDestroyContext(_display->eglDisplay(), _eglContext);
        eglDestroySurface(_display->eglDisplay(), _eglSurface);

        wl_egl_window_destroy(_wlEglWindow);

        xdg_surface_destroy(_xdgSurface);
        wl_surface_destroy(_wlSurface);
    }

    void Surface::setNeedsRedraw()
    {
        _needsRedraw = true;
        renderFrame();
    }

    void Surface::renderFrame()
    {
        if (_renderingFrame)
        {
            _frameScheduled = true;
            return;
        }

        _renderingFrame = true;

        _frameCallback = wl_surface_frame(_wlSurface);
        wl_callback_add_listener(_frameCallback, &_frameCallbackListener, this);

        eglMakeCurrent(_display->eglDisplay(), _eglSurface, _eglSurface, _eglContext);
        eglSwapInterval(_display->eglDisplay(), 0);

        glViewport(0, 0, _size.width, _size.height);

        if (_needsRedraw)
            draw();

        eglSwapBuffers(_display->eglDisplay(), _eglSurface);
    }

    void Surface::draw()
    {
        float red = rand() % 256 / 255.0f;
        float green = rand() % 256 / 255.0f;
        float blue = rand() % 256 / 255.0f;

        glClearColor(red, green, blue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        _needsRedraw = false;
    }

    void Surface::didConfigure(uint32_t serial)
    {
        xdg_surface_ack_configure(_xdgSurface, serial);

        setNeedsRedraw();
    }

    void Surface::frameDidFinish(uint32_t time)
    {
        wl_callback_destroy(_frameCallback);
        _frameCallback = nullptr;

        _renderingFrame = false;

        if (_frameScheduled)
        {
            _frameScheduled = false;
            renderFrame();
        }
    }
}
