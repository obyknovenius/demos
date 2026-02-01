#include "Seat.h"

#include "Display.h"

namespace Platform::Wayland
{
    NonNull<RefPtr<Seat>> Seat::create(NonNull<wl_seat*> wlSeat, NonNull<RefPtr<Display>> display)
    {
        return adopt(new Seat(wlSeat, display));
    }

    const wl_seat_listener Seat::_wlSeatListener = {
        .capabilities = [](void* data, wl_seat* wlSeat, uint32_t capabilities)
        {
            auto* seat = reinterpret_cast<Seat*>(data);
            seat->capabilitiesDidChange(capabilities);
        },
        .name = nullptr
    };

    Seat::Seat(NonNull<wl_seat*> wlSeat, NonNull<RefPtr<Display>> display) :
        _wlSeat { wlSeat },
        _display { display }
    {
        wl_seat_add_listener(_wlSeat, &_wlSeatListener, this);
    }

    Seat::~Seat()
    {
        wl_seat_destroy(_wlSeat);
    }

    void Seat::capabilitiesDidChange(uint32_t capabilities)
    {
        bool havePointer = capabilities & WL_SEAT_CAPABILITY_POINTER;

        if (havePointer && !_pointer)
            _pointer = Pointer::create(wl_seat_get_pointer(_wlSeat), this);
        else if (!havePointer && _pointer)
            _pointer = nullptr;
    }
}
