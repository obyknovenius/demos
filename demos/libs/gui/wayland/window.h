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
        static nonnull_ref_ptr<window> create(const nonnull_ref_ptr<display>& display)
        {
            return adopt(*new window(display));
        }

        void close() override;

        xdg_toplevel* get_xdg_toplevel() { return m_xdg_toplevel; }

    private:
        static const xdg_surface_listener s_xdg_surface_listener;
        static const wl_buffer_listener s_wl_buffer_listener;

        window(const nonnull_ref_ptr<display>& display);
        ~window();

        void on_surface_configure(xdg_surface* xdg_surface, uint32_t serial);
        void on_buffer_release(wl_buffer* buffer);

        weak_ptr<display> m_display;

        wl_surface* m_wl_surface {};

        int m_shm_fd { -1 };
        wl_shm_pool* m_wl_shm_pool {};

        wl_buffer* m_wl_buffer {};

        xdg_surface* m_xdg_surface {};
        xdg_toplevel* m_xdg_toplevel {};

        bool m_closed { false };
    };
}
