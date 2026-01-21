#pragma once

#include "../Size.h"
#include <Core/NonNull.h>
#include <Core/RefCounted.h>
#include <Core/RefPtr.h>
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
        NonNull<RefPtr<Display>> display() const { return _display; }

        EGLConfig eglConfig() const { return _eglConfig; }
        EGLSurface eglSurface() const { return _eglSurface; }

        Size size() const { return _size; }

        std::function<void()> onPointerMotion;

    protected:
        Surface(NonNull<RefPtr<Display>> display);
        ~Surface() override;

        NonNull<RefPtr<Display>> _display;

        wl_surface* _wlSurface {};
        xdg_surface* _xdgSurface {};

        wl_egl_window* _wlEglWindow {};

        EGLConfig _eglConfig {};
        EGLSurface _eglSurface { EGL_NO_SURFACE };

        Size _size { 800, 600 };

    private:
        static const wl_surface_listener _wlSurfaceListener;
        static const xdg_surface_listener _xdgSurfaceListener;

        void onWlSurfaceEnter(wl_surface* wlSurface, wl_output* wlOutput);
        void onWlSurfaceLeave(wl_surface* wlSurface, wl_output* wlOutput);
        void onWlSurfacePreferredBufferScale(wl_surface* wlSurface, int32_t scale);
        void onWlSurfacePreferredBufferTransform(wl_surface* wlSurface, uint32_t transform);

        void onXdgSurfaceConfigure(xdg_surface* xdgSurface, uint32_t serial);
    };
}
