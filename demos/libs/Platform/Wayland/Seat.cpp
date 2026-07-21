#include "Seat.h"

#include "Display.h"

namespace Platform::Wayland
{
    const wl_seat_listener Seat::_wlSeatListener = {
        .capabilities = [](void* data, wl_seat* wlSeat, uint32_t capabilities)
        {
            auto* seat = reinterpret_cast<Seat*>(data);
            seat->capabilitiesDidChange(capabilities);
        },
        .name = [](void* data, wl_seat* wlSeat, const char* name)
        {
        },
    };

    Seat::Seat(NonNull<wl_seat*> wlSeat, NonNull<StrongPtr<Display>> display) :
        _wlSeat(wlSeat),
        _display(display)
    {
        wl_seat_add_listener(_wlSeat, &_wlSeatListener, this);
    }

    Seat::~Seat()
    {
        _pointer = nullptr;

        wl_seat_destroy(_wlSeat);
    }

    void Seat::capabilitiesDidChange(uint32_t capabilities)
    {
        bool havePointer = capabilities & WL_SEAT_CAPABILITY_POINTER;

        if (havePointer && !_pointer)
            _pointer = makeStrong<Pointer>(wl_seat_get_pointer(_wlSeat), this);
        else if (!havePointer && _pointer)
            _pointer = nullptr;
    }
}
