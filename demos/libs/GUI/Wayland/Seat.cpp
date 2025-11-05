#include "Seat.h"

#include "Display.h"
#include "Pointer.h"

namespace GUI::Wayland
{
    const wl_seat_listener Seat::_wlSeatListener = {
        .capabilities = [](void* data, wl_seat* wlSeat, uint32_t capabilities)
        {
            auto* seat = reinterpret_cast<Seat*>(data);
            seat->onCapabilities(capabilities);
        },
        .name = [](void* data, wl_seat* wlSeat, const char* name)
        {
        }
    };

    Seat::Seat(wl_seat* wlSeat, const NonnullRefPtr<Display>& display) :
        _wlSeat { wlSeat },
        _display { display }
    {
        wl_seat_add_listener(_wlSeat, &_wlSeatListener, this);
    }

    Seat::~Seat()
    {
        _pointer = nullptr;

        wl_seat_release(_wlSeat);
    }

    void Seat::onCapabilities(uint32_t capabilities)
    {
        bool havePointer = capabilities & WL_SEAT_CAPABILITY_POINTER;

        if (havePointer && !_pointer)
            _pointer = Pointer::make(wl_seat_get_pointer(_wlSeat), *this);
        else if (!havePointer && _pointer)
            _pointer = nullptr;
    }
}
