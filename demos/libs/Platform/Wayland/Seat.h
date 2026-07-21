#pragma once

#include "Pointer.h"
#include <Foundation/NonNull.h>
#include <Foundation/Object.h>
#include <Foundation/StrongPtr.h>
#include <wayland-client.h>

namespace Platform::Wayland
{
    class Display;

    class Seat final : public Object
    {
    public:
        Seat(NonNull<wl_seat*> wlSeat, NonNull<StrongPtr<Display>> display);

    private:
        static const wl_seat_listener _wlSeatListener;

        ~Seat() override;

        void capabilitiesDidChange(uint32_t capabilities);

        NonNull<wl_seat*> _wlSeat;

        NonNull<StrongPtr<Display>> _display;

        StrongPtr<Pointer> _pointer = nullptr;
    };
}
