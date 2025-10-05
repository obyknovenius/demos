#pragma once

#include "../display.h"
#include <wayland-client.h>
#include <xdg-shell-client-protocol.h>

namespace gui::wayland
{   
    class seat;

    class display final : public gui::display
    {
    public:
        static ref_ptr<display> connect();

        nonnull_ref_ptr<gui::window> create_window() override;

        wl_compositor* get_wl_compositor() { return m_wl_compositor; }
        xdg_wm_base* get_xdg_wm_base() { return m_xdg_wm_base; }
        wl_shm* get_wl_shm() { return m_wl_shm; }

    private:
        static nonnull_ref_ptr<display> create(wl_display* wl_display)
        {
            return adopt(*new display(wl_display));
        }

        display(wl_display* wl_display);
        ~display();

        static const wl_registry_listener s_wl_registry_listener;
        static const xdg_wm_base_listener s_xdg_wm_base_listener;

        void on_registry_global(wl_registry* registry, uint32_t name, const char* interface, uint32_t version);
        void on_wm_ping(xdg_wm_base* xdg_wm_base, uint32_t serial);

        wl_display* m_wl_display {};
        wl_registry* m_wl_registry {};
        wl_compositor* m_wl_compositor {};
        xdg_wm_base* m_xdg_wm_base {};
        wl_shm* m_wl_shm {};

        ref_ptr<seat> m_seat;
    };
}
