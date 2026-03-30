#pragma once

#include "Pointer.h"
#include <Foundation/EnableWeakPtr.h>
#include <Foundation/NonNull.h>
#include <Foundation/NonNullRefPtr.h>
#include <Foundation/RefCounted.h>
#include <Foundation/RefPtr.h>
#include <Foundation/WeakPtr.h>
#include <wayland-client.h>

namespace Platform::Wayland
{
    class Display;

    class Seat final : public RefCounted, public EnableWeakPtr<Seat>
    {
    public:
        static NonNullRefPtr<Seat> create(NonNull<wl_seat*> wlSeat, NonNullRefPtr<Display> display);

    private:
        static const wl_seat_listener _wlSeatListener;

        Seat(NonNull<wl_seat*> wlSeat, NonNullRefPtr<Display> display);
        ~Seat() override;

        void capabilitiesDidChange(uint32_t capabilities);

        NonNull<wl_seat*> _wlSeat;

        WeakPtr<Display> _display {};

        RefPtr<Pointer> _pointer {};
    };
}
