#pragma once

#include <Core/NonNull.h>
#include <Core/RefCounted.h>
#include <Core/RefPtr.h>
#include <Core/Weakable.h>
#include <Core/WeakPtr.h>
#include <wayland-client.h>

namespace Gfx::Wayland
{
    class Display;
    class Pointer;

    class Seat final : public RefCounted, public Weakable
    {
    public:
        static NonNull<RefPtr<Seat>> create(NonNull<wl_seat*> wlSeat, NonNull<RefPtr<Display>> display)
        {
            return adopt(new Seat(wlSeat, display));
        }

        NonNull<wl_seat*> wlSeat() { return _wlSeat; }

        WeakPtr<Display> display() { return _display; }

    private:
        static const wl_seat_listener _wlSeatListener;

        Seat(NonNull<wl_seat*> wlSeat, NonNull<RefPtr<Display>> display);
        ~Seat() override;

        void onWlSeatCapabilities(wl_seat* wlSeat, uint32_t capabilities);
        void onWlSeatName(wl_seat* wlSeat, const char* name);

        NonNull<wl_seat*> _wlSeat;

        WeakPtr<Display> _display;

        RefPtr<Pointer> _pointer;
    };
}
