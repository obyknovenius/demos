#pragma once

#include <core/weakable.h>
#include <core/weak_ptr.h>
#include <wayland-client.h>

namespace gui::wayland
{
    class display;
    class pointer;

    class seat final : public weakable
    {
    public:
        static nonnull_ref_ptr<seat> create(wl_seat* wl_seat, const nonnull_ref_ptr<display>& display)
        {
            return adopt(*new seat(wl_seat, display));
        }

        ref_ptr<display> get_display() { return m_display.strong_ref(); }

        wl_seat* get_wl_seat() { return m_wl_seat; }

    private:
        static const wl_seat_listener s_wl_seat_listener;

        seat(wl_seat* wl_seat, const nonnull_ref_ptr<display>& display);
        ~seat();

        void on_capabilities(uint32_t capabilities);

        wl_seat* m_wl_seat {};

        weak_ptr<display> m_display;

        ref_ptr<pointer> m_pointer;
    };
}
