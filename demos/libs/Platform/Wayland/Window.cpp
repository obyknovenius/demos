#include "Window.h"

#include "Display.h"
#include <GLES3/gl3.h>

namespace Platform::Wayland
{
    NonNull<RefPtr<Window>> Window::create(NonNull<RefPtr<Display>> display, Gfx::Size size)
    {
        return adopt(new Window(display, size));
    }

    const xdg_surface_listener Window::_xdgSurfaceListener = {
        .configure = [](void* data, xdg_surface* xdgSurface, uint32_t serial)
        {
            auto* window = reinterpret_cast<Window*>(data);
            window->didConfigure(serial);
        }
    };

    const wl_callback_listener Window::_frameCallbackListener = {
        .done = [](void* data, wl_callback* wlCallback, uint32_t time)
        {
            auto* window = reinterpret_cast<Window*>(data);
            window->drawNextFrameIfNeeded();
        }
    };

    Window::Window(NonNull<RefPtr<Display>> display, Gfx::Size size)
        : Platform::Window { size }
        , _display { display }
    {
        _wlSurface = wl_compositor_create_surface(_display->wlCompositor());
        wl_surface_set_user_data(_wlSurface, this);

        _xdgSurface = xdg_wm_base_get_xdg_surface(_display->xdgWmBase(), _wlSurface);
        xdg_surface_add_listener(_xdgSurface, &_xdgSurfaceListener, this);

        _xdgToplevel = xdg_surface_get_toplevel(_xdgSurface);

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

        wl_surface_commit(_wlSurface);
    }

    Window::~Window()
    {
        if (_frameCallback)
            wl_callback_destroy(_frameCallback);

        eglDestroyContext(_display->eglDisplay(), _eglContext);
        eglDestroySurface(_display->eglDisplay(), _eglSurface);

        wl_egl_window_destroy(_wlEglWindow);

        xdg_surface_destroy(_xdgSurface);
        wl_surface_destroy(_wlSurface);
    }

    void Window::setNeedsDraw()
    {
        if (!_needsDraw)
        {
            _needsDraw = true;

            if (!_drawingFrame)
                drawFrame();
            else
                setNeedsDrawNextFrame();
        }
    }

    void Window::didConfigure(uint32_t serial)
    {
        xdg_surface_ack_configure(_xdgSurface, serial);

        setNeedsDraw();
    }

    void Window::drawFrame()
    {
        _drawingFrame = true;

        _frameCallback = wl_surface_frame(_wlSurface);
        wl_callback_add_listener(_frameCallback, &_frameCallbackListener, this);

        eglMakeCurrent(_display->eglDisplay(), _eglSurface, _eglSurface, _eglContext);
        eglSwapInterval(_display->eglDisplay(), 0);

        glViewport(0, 0, _size.width, _size.height);

        drawIfNeeded();

        eglSwapBuffers(_display->eglDisplay(), _eglSurface);
    }

    void Window::setNeedsDrawNextFrame()
    {
        _needsDrawNextFrame = true;
    }

    void Window::drawNextFrameIfNeeded()
    {
        wl_callback_destroy(_frameCallback);
        _frameCallback = nullptr;

        _drawingFrame = false;

        if (_needsDrawNextFrame)
        {
            _needsDrawNextFrame = false;
            drawFrame();
        }
    }
}
