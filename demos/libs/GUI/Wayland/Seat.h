#pragma once

#include "Forward.h"
#include <Foundation/Foundation.h>
#include <wayland-client.h>

namespace GUI::Wayland
{
    class Seat final : public Object
    {
    public:
        static NonNull<StrongPtr<Seat>> make(NonNull<wl_seat*> wlSeat, StrongPtr<Display> display)
        {
            return StrongPtr<Seat>::adopt(new Seat(wlSeat, display));
        }

        // TODO: Return WeakPtr
        StrongPtr<Display> display() { return _display; }

        NonNull<wl_seat*> wlSeat() { return _wlSeat; }

        StrongPtr<Pointer> pointer();

    private:
        static const wl_seat_listener _wlSeatListener;

        Seat(NonNull<wl_seat*> wlSeat, StrongPtr<Display> display);
        ~Seat();

        void onCapabilities(uint32_t capabilities);

        NonNull<wl_seat*> _wlSeat;

        WeakPtr<Display> _display;

        StrongPtr<Pointer> _pointer;
    };
}
