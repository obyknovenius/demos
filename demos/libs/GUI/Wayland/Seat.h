#pragma once

#include <Core/NonNull.h>
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
        static NonNull<RefPtr<Seat>> make(NonNull<wl_seat*> wlSeat, const RefPtr<Display>& display);

        RefPtr<Display> display();

        NonNull<wl_seat*> wlSeat();
        RefPtr<Pointer> pointer();

    private:
        static const wl_seat_listener _wlSeatListener;

        Seat(NonNull<wl_seat*> wlSeat, const RefPtr<Display>& display);
        ~Seat();

        void onCapabilities(uint32_t capabilities);

        NonNull<wl_seat*> _wlSeat;

        WeakPtr<Display> _display;

        RefPtr<Pointer> _pointer;
    };

    inline NonNull<RefPtr<Seat>> Seat::make(NonNull<wl_seat*> wlSeat, const RefPtr<Display>& display)
    {
        return adopt(new Seat(wlSeat, display));
    }

    inline RefPtr<Display> Seat::display()
    {
        return _display.strong();
    }

    inline NonNull<wl_seat*> Seat::wlSeat()
    {
        return _wlSeat;
    }
}
