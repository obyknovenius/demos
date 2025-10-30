#pragma once

#include <Core/Weakable.h>
#include <Core/WeakPtr.h>
#include <wayland-client.h>

namespace gui::wayland
{
    class display;
    class pointer;

    class seat final : public Weakable
    {
    public:
        static NonnullRefPtr<seat> make(wl_seat* wl_seat, const NonnullRefPtr<display>& display);

        RefPtr<display> get_display() { return m_display.strong(); }

        wl_seat* get_wl_seat() { return m_wl_seat; }

    private:
        static const wl_seat_listener s_wl_seat_listener;

        seat(wl_seat* wl_seat, const NonnullRefPtr<display>& display);
        ~seat();

        void on_capabilities(uint32_t capabilities);

        wl_seat* m_wl_seat {};

        WeakPtr<display> m_display;

        RefPtr<pointer> m_pointer;
    };

    inline NonnullRefPtr<seat> seat::make(wl_seat* wl_seat, const NonnullRefPtr<display>& display)
    {
        return adopt(*new seat(wl_seat, display));
    }
}
