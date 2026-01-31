#include "Pointer.h"

#include "Seat.h"

namespace Platform::Wayland
{
    NonNull<RefPtr<Pointer>> Pointer::create(NonNull<wl_pointer*> wlPointer, NonNull<RefPtr<Seat>> seat)
    {
        return adopt(new Pointer(wlPointer, seat));
    }

    const wl_pointer_listener Pointer::_wlPointerListener = {
        .enter = [](void* data, wl_pointer* wlPointer, uint32_t serial, wl_surface* surface, wl_fixed_t x, wl_fixed_t y)
        {
        },
        .leave = [](void* data, wl_pointer* wlPointer, uint32_t serial, wl_surface* surface)
        {
        },
        .motion = [](void* data, wl_pointer* wlPointer, uint32_t time, wl_fixed_t x, wl_fixed_t y)
        {
        },
        .button = [](void* data, wl_pointer* wlPointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state)
        {
        },
        .axis = [](void* data, wl_pointer* wlPointer, uint32_t time, uint32_t axis, wl_fixed_t value)
        {
        },
        .frame = [](void* data, wl_pointer* wlPointer)
        {
        },
        .axis_source = [](void* data, wl_pointer* wlPointer, uint32_t axis_source)
        {
        },
        .axis_stop = [](void* data, wl_pointer* wlPointer, uint32_t time, uint32_t axis_stop)
        {
        },
        .axis_discrete = [](void* data, wl_pointer* wlPointer, uint32_t axis, int32_t discrete)
        {
        }
    };

    Pointer::Pointer(NonNull<wl_pointer*> wlPointer, NonNull<RefPtr<Seat>> seat) :
        _wlPointer { wlPointer },
        _seat { seat }
    {
        wl_pointer_add_listener(_wlPointer, &_wlPointerListener, this);
    }

    Pointer::~Pointer()
    {
        wl_pointer_destroy(_wlPointer);
    }
}
