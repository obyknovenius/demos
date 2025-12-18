#pragma once

#include "../Window.h"
#include <Core/WeakPtr.h>
#include <wayland-client.h>
#include <xdg-shell-client-protocol.h>

namespace GUI::Wayland
{
    class Display;

    class Window final : public GUI::Window
    {
    public:
        static NonnullRefPtr<Window> make();

        void setMaximized(bool maximized) override;

        void close() override;

        void beginMove() override;
        void beginResize(Edges edges) override;

        void setCursor(Cursor cursor) override;

        xdg_toplevel* xdgToplevel();

    private:
        static const wl_surface_listener _wlSurfaceListener;
        static const xdg_surface_listener _xdgSurfaceListener;
        static const xdg_toplevel_listener _xdgToplevelListener;
        static const wl_buffer_listener _wlBufferListener;

        Window(const NonnullRefPtr<Display>& display);
        ~Window();

        void onSurfaceEnter(wl_surface* wlSurface, wl_output* output);
        void onSurfaceLeave(wl_surface* wlSurface, wl_output* output);
        void onSurfacePreferredBufferScale(wl_surface* wlSurface, int32_t scale);
        void onSurfacePreferredBufferTransform(wl_surface* wlSurface, uint32_t transform);

        void onSurfaceConfigure(xdg_surface* xdgSurface, uint32_t serial);
        void onToplevelConfigure(xdg_toplevel* xdgToplevel, int32_t width, int32_t height, wl_array* states);
        void onToplevelClose(xdg_toplevel* xdgToplevel);
        void onBufferRelease(wl_buffer* buffer);

        WeakPtr<Display> _display;

        wl_surface* _wlSurface {};

        wl_shm_pool* _wlShmPool {};

        wl_buffer* _wlBuffer {};

        xdg_surface* _xdgSurface {};
        xdg_toplevel* _xdgToplevel {};

        int _scaleFactor { 1 };

        bool _closed { false };
    };

    inline xdg_toplevel* GUI::Wayland::Window::xdgToplevel()
    {
        return _xdgToplevel;
    }
}
