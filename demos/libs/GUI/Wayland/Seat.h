#pragma once

#include <Core/Weakable.h>
#include <Core/WeakPtr.h>
#include <wayland-client.h>

namespace GUI::Wayland
{
    class Display;
    class Pointer;

    class Seat final : public Weakable
    {
    public:
        static NonnullRefPtr<Seat> make(wl_seat* wlSeat, const NonnullRefPtr<Display>& display);

        RefPtr<Display> getDisplay();

        wl_seat* getWlSeat();

    private:
        static const wl_seat_listener _wlSeatListener;

        Seat(wl_seat* wlSeat, const NonnullRefPtr<Display>& display);
        ~Seat();

        void onCapabilities(uint32_t capabilities);

        wl_seat* _wl_seat {};

        WeakPtr<Display> _display;

        RefPtr<Pointer> _pointer;
    };

    inline NonnullRefPtr<Seat> Seat::make(wl_seat* wlSeat, const NonnullRefPtr<Display>& display)
    {
        return adopt(*new Seat(wlSeat, display));
    }

    inline RefPtr<Display> Seat::getDisplay()
    {
        return _display.strong();
    }

    inline wl_seat* Seat::getWlSeat()
    {
        return _wl_seat;
    }
}
