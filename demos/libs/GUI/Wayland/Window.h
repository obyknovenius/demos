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

        void beginMove() override;
        void beginResize(Edges edges) override;

        void close() override;

        void setCursor(Cursor cursor) override;

        xdg_toplevel* xdgToplevel();

    private:
        static const xdg_surface_listener _xdgSurfaceListener;
        static const xdg_toplevel_listener _xdgToplevelListener;
        static const wl_buffer_listener _wlBufferListener;

        Window(const NonnullRefPtr<Display>& display);
        ~Window();

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

        bool _closed { false };
    };

    inline xdg_toplevel* GUI::Wayland::Window::xdgToplevel()
    {
        return _xdgToplevel;
    }
}
