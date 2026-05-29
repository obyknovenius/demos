#pragma once

#include "Pointer.h"
#include <Foundation/Object.h>
#include <wayland-client.h>

namespace Platform::Wayland
{
    class Display;

    class Seat final : public Object
    {
    public:
        static NonNull<RefPtr<Seat>> create(NonNull<wl_seat*> wlSeat, NonNull<RefPtr<Display>> display);

    private:
        static const wl_seat_listener _wlSeatListener;

        Seat(NonNull<wl_seat*> wlSeat, NonNull<RefPtr<Display>> display);
        ~Seat() override;

        void capabilitiesDidChange(uint32_t capabilities);

        NonNull<wl_seat*> _wlSeat;

        WeakPtr<Display> _display {};

        RefPtr<Pointer> _pointer {};
    };
}
