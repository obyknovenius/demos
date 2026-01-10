#pragma once

#include "Forward.h"
#include <Core/NonNull.h>
#include <Core/Weakable.h>
#include <Core/WeakPtr.h>
#include <wayland-client.h>

namespace GUI::Wayland
{
    class Seat final : public Weakable
    {
    public:
        static NonNull<RefPtr<Seat>> make(NonNull<wl_seat*> wlSeat, RefPtr<Display> display)
        {
            return adopt(new Seat(wlSeat, display));
        }

        RefPtr<Display> display() { return _display.strong(); }

        NonNull<wl_seat*> wlSeat() { return _wlSeat; }

        RefPtr<Pointer> pointer();

    private:
        static const wl_seat_listener _wlSeatListener;

        Seat(NonNull<wl_seat*> wlSeat, RefPtr<Display> display);
        ~Seat();

        void onCapabilities(uint32_t capabilities);

        NonNull<wl_seat*> _wlSeat;

        WeakPtr<Display> _display;

        RefPtr<Pointer> _pointer;
    };
}
