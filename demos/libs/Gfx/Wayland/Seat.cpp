#include "Seat.h"

#include "Display.h"
#include "Pointer.h"

namespace Gfx::Wayland
{
    const wl_seat_listener Seat::_wlSeatListener = {
        .capabilities = [](void* data, wl_seat* wlSeat, uint32_t capabilities)
        {
            auto* seat = reinterpret_cast<Seat*>(data);
            seat->onWlSeatCapabilities(wlSeat, capabilities);
        },
        .name = [](void* data, wl_seat* wlSeat, const char* name)
        {
            auto* seat = reinterpret_cast<Seat*>(data);
            seat->onWlSeatName(wlSeat, name);
        }
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

    void Seat::onWlSeatCapabilities(wl_seat* wlSeat, uint32_t capabilities)
    {
        bool havePointer = capabilities & WL_SEAT_CAPABILITY_POINTER;

        if (havePointer && !_pointer)
            _pointer = Pointer::create(wl_seat_get_pointer(_wlSeat), { this });
        else if (!havePointer && _pointer)
            _pointer = nullptr;
    }

    void Seat::onWlSeatName(wl_seat* wlSeat, const char* name)
    {
    }
}
