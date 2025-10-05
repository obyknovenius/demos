#pragma once

#include <core/weakable.h>
#include <core/weak_ptr.h>
#include <wayland-client.h>

namespace gui
{
    class wayland_display;
    class wayland_pointer;

    class wayland_seat final : public weakable
    {
    public:
        static nonnull_ref_ptr<wayland_seat> create(wl_seat* wl_seat, const nonnull_ref_ptr<wayland_display>& display)
        {
            return adopt(*new wayland_seat(wl_seat, display));
        }

        ref_ptr<wayland_display> get_display() { return m_display.strong_ref(); }

        wl_seat* get_wl_seat() { return m_wl_seat; }

    private:
        static const wl_seat_listener s_wl_seat_listener;

        wayland_seat(wl_seat* wl_seat, const nonnull_ref_ptr<wayland_display>& display);
        ~wayland_seat();

        void on_capabilities(uint32_t capabilities);

        wl_seat* m_wl_seat {};

        weak_ptr<wayland_display> m_display;

        ref_ptr<wayland_pointer> m_pointer;
    };
}
