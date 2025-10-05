#pragma once

#include "../display.h"
#include <wayland-client.h>
#include <xdg-shell-client-protocol.h>

namespace gui
{
    class wayland_seat;

    class wayland_display final : public display
    {
    public:
        static ref_ptr<wayland_display> connect();

        nonnull_ref_ptr<window> create_window() override;

        wl_compositor* get_wl_compositor() { return m_wl_compositor; }
        xdg_wm_base* get_xdg_wm_base() { return m_xdg_wm_base; }
        wl_shm* get_wl_shm() { return m_wl_shm; }

    private:
        static nonnull_ref_ptr<wayland_display> create(wl_display* wl_display)
        {
            return adopt(*new wayland_display(wl_display));
        }

        wayland_display(wl_display* wl_display);
        ~wayland_display();

        static const wl_registry_listener s_wl_registry_listener;
        static const xdg_wm_base_listener s_xdg_wm_base_listener;

        void on_registry_global(wl_registry* registry, uint32_t name, const char* interface, uint32_t version);
        void on_wm_ping(xdg_wm_base* xdg_wm_base, uint32_t serial);

        wl_display* m_wl_display {};
        wl_registry* m_wl_registry {};
        wl_compositor* m_wl_compositor {};
        xdg_wm_base* m_xdg_wm_base {};
        wl_shm* m_wl_shm {};

        ref_ptr<wayland_seat> m_seat;
    };
}
