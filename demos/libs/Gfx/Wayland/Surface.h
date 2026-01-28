#pragma once

#include "../Size.h"
#include <Core/NonNull.h>
#include <Core/RefCounted.h>
#include <Core/RefPtr.h>
#include <Core/Weakable.h>
#include <Core/WeakPtr.h>
#include <EGL/egl.h>
#include <functional>
#include <wayland-client.h>
#include <wayland-egl.h>
#include <xdg-shell-client-protocol.h>

namespace Gfx::Wayland
{
    class Display;

    class Surface : public RefCounted
    {
    public:
        Size size() const { return _size; }

        void setNeedsRedraw();

        std::function<void()> onPointerMotion;

    protected:
        Surface(NonNull<RefPtr<Display>> display);
        ~Surface() override;

        void draw();

        NonNull<RefPtr<Display>> _display;

        wl_surface* _wlSurface {};
        xdg_surface* _xdgSurface {};

        wl_egl_window* _wlEglWindow {};

        EGLConfig _eglConfig {};
        EGLSurface _eglSurface { EGL_NO_SURFACE };
        EGLContext _eglContext { EGL_NO_CONTEXT };

        Size _size { 800, 600 };

    private:
        static const xdg_surface_listener _xdgSurfaceListener;
        static const wl_callback_listener _frameCallbackListener;

        void didConfigure(uint32_t serial);

        void renderFrame();
        void frameDidFinish(uint32_t time);

        bool _needsRedraw { false };

        bool _renderingFrame { false };
        bool _frameScheduled { false };

        wl_callback* _frameCallback {};
    };
}
