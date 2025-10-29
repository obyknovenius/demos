#pragma once

#include "../window.h"
#include <core/weak_ptr.h>
#include <wayland-client.h>
#include <xdg-shell-client-protocol.h>

namespace gui::wayland
{
    class display;

    class window final : public gui::window
    {
    public:
        window();
        ~window();

        void close() override;

        xdg_toplevel* get_xdg_toplevel() { return m_xdg_toplevel; }

    private:
        static const xdg_surface_listener s_xdg_surface_listener;
        static const xdg_toplevel_listener s_xdg_toplevel_listener;
        static const wl_buffer_listener s_wl_buffer_listener;

        void on_surface_configure(xdg_surface* xdg_surface, uint32_t serial);
        void on_toplevel_configure(xdg_toplevel* xdg_toplevel, int32_t width, int32_t height, wl_array* states);
        void on_toplevel_close(xdg_toplevel* xdg_toplevel);
        void on_buffer_release(wl_buffer* buffer);

        wl_surface* m_wl_surface {};

        int m_shm_fd { -1 };
        wl_shm_pool* m_wl_shm_pool {};

        wl_buffer* m_wl_buffer {};

        xdg_surface* m_xdg_surface {};
        xdg_toplevel* m_xdg_toplevel {};

        bool m_closed { false };
    };
}
