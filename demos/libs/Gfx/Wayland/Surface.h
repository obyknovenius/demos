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
        class Delegate : public RefCounted, public Weakable
        {
        public:
            virtual void surfaceDidFinishFrame(RefPtr<Surface> surface) = 0;
        };

        NonNull<RefPtr<Display>> display() const { return _display; }

        Size size() const { return _size; }

        void setDelegate(RefPtr<Delegate> delegate) { _delegate = delegate; }
        WeakPtr<Delegate> delegate() const { return _delegate; }

        void beginFrame();
        void endFrame();

        std::function<void()> onPointerMotion;

    protected:
        Surface(NonNull<RefPtr<Display>> display);
        ~Surface() override;

        void didFinishFrame();

        NonNull<RefPtr<Display>> _display;

        wl_surface* _wlSurface {};
        xdg_surface* _xdgSurface {};

        wl_egl_window* _wlEglWindow {};

        EGLConfig _eglConfig {};
        EGLSurface _eglSurface { EGL_NO_SURFACE };
        EGLContext _eglContext { EGL_NO_CONTEXT };

        Size _size { 800, 600 };

        WeakPtr<Delegate> _delegate;

    private:
        static const wl_surface_listener _wlSurfaceListener;
        static const wl_callback_listener _wlFrameCallbackListener;
        static const xdg_surface_listener _xdgSurfaceListener;

        wl_callback* _wlFrameCallback {};

        void onWlSurfaceEnter(wl_surface* wlSurface, wl_output* wlOutput);
        void onWlSurfaceLeave(wl_surface* wlSurface, wl_output* wlOutput);
        void onWlSurfacePreferredBufferScale(wl_surface* wlSurface, int32_t scale);
        void onWlSurfacePreferredBufferTransform(wl_surface* wlSurface, uint32_t transform);

        void onXdgSurfaceConfigure(xdg_surface* xdgSurface, uint32_t serial);
    };
}
