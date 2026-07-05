#pragma once

#include "../Window.h"
#include <EGL/egl.h>
#include <Foundation/NonNull.h>
#include <Foundation/StrongPtr.h>
#include <wayland-client.h>
#include <wayland-egl.h>
#include <xdg-shell-client-protocol.h>

namespace Platform::Wayland
{
    class Display;

    class Window : public Platform::Window
    {
    public:
        Window(Gfx::Size size = { 800, 600 });

        void setNeedsDraw() override;

    private:
        static const xdg_surface_listener _xdgSurfaceListener;
        static const wl_callback_listener _frameCallbackListener;

        ~Window() override;

        void didConfigure(uint32_t serial);

        void drawFrame();
        void setNeedsDrawNextFrame();
        void drawNextFrameIfNeeded();

        NonNull<StrongPtr<Display>> _display;

        wl_surface* _wlSurface = nullptr;
        xdg_surface* _xdgSurface = nullptr;
        xdg_toplevel* _xdgToplevel = nullptr;

        wl_egl_window* _wlEglWindow = nullptr;

        EGLConfig _eglConfig = nullptr;
        EGLSurface _eglSurface = EGL_NO_SURFACE;
        EGLContext _eglContext = EGL_NO_CONTEXT;

        bool _drawingFrame = false;
        bool _needsDrawNextFrame = false;

        wl_callback* _frameCallback = nullptr;
    };
}
