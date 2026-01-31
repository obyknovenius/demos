#pragma once

#include "Pointer.h"
#include <Core/NonNull.h>
#include <Core/RefCounted.h>
#include <Core/RefPtr.h>
#include <Core/Weakable.h>
#include <Core/WeakPtr.h>
#include <wayland-client.h>

namespace Platform::Wayland
{
    class Display;

    class Seat final : public RefCounted, public Weakable
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
