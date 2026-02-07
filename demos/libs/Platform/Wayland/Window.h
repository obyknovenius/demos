#pragma once

#include "../Window.h"
#include <Core/NonNull.h>
#include <Core/RefPtr.h>
#include <EGL/egl.h>
#include <wayland-client.h>
#include <wayland-egl.h>
#include <xdg-shell-client-protocol.h>

namespace Platform::Wayland
{
    class Display;

    class Window : public Platform::Window
    {
    public:
        static NonNull<RefPtr<Window>> create(NonNull<RefPtr<Display>> display, Gfx::Size size = { 800, 600 });

        void setNeedsLayout() override;
        void setNeedsDraw() override;

    private:
        static const xdg_surface_listener _xdgSurfaceListener;

        Window(NonNull<RefPtr<Display>>display, Gfx::Size size);
        ~Window() override;

        void didConfigure(uint32_t serial);

        void renderFrame();

        NonNull<RefPtr<Display>> _display;

        wl_surface* _wlSurface {};
        xdg_surface* _xdgSurface {};
        xdg_toplevel* _xdgToplevel {};

        wl_egl_window* _wlEglWindow {};

        EGLConfig _eglConfig {};
        EGLSurface _eglSurface { EGL_NO_SURFACE };
        EGLContext _eglContext { EGL_NO_CONTEXT };
    };
}
