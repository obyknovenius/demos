#pragma once

#include "Pointer.h"
#include <Foundation/Object.h>
#include <Foundation/StrongPtr.h>
#include <wayland-client.h>

namespace Platform::Wayland
{
    class Display;

    class Seat final : public Object
    {
    public:
        Seat(wl_seat* wlSeat, StrongPtr<Display> display);

    private:
        static const wl_seat_listener _wlSeatListener;

        ~Seat() override;

        void capabilitiesDidChange(uint32_t capabilities);

        wl_seat* _wlSeat = nullptr;

        WeakPtr<Display> _display = nullptr;

        StrongPtr<Pointer> _pointer = nullptr;
    };
}
