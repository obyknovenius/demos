#include "Pointer.h"

#include "Seat.h"
#include "Surface.h"
#include "Display.h"

namespace Gfx::Wayland
{
    const wl_pointer_listener Pointer::_wlPointerListener = {
        .enter = [](void* data, wl_pointer* wlPointer, uint32_t serial, wl_surface* surface, wl_fixed_t x, wl_fixed_t y)
        {
            auto* pointer = reinterpret_cast<Pointer*>(data);
            pointer->onWlPointerEnter(wlPointer, serial, surface, x, y);
        },
        .leave = [](void* data, wl_pointer* wlPointer, uint32_t serial, wl_surface* surface)
        {
            auto* pointer = reinterpret_cast<Pointer*>(data);
            pointer->onWlPointerLeave(wlPointer, serial, surface);
        },
        .motion = [](void* data, wl_pointer* wlPointer, uint32_t time, wl_fixed_t x, wl_fixed_t y)
        {
            auto* pointer = reinterpret_cast<Pointer*>(data);
            pointer->onWlPointerMotion(wlPointer, time, x, y);
        },
        .button = [](void* data, wl_pointer* wlPointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state)
        {
            auto* pointer = reinterpret_cast<Pointer*>(data);
            pointer->onWlPointerButton(wlPointer, serial, time, button, state);
        },
        .axis = [](void* data, wl_pointer* wlPointer, uint32_t time, uint32_t axis, wl_fixed_t value)
        {
            auto* pointer = reinterpret_cast<Pointer*>(data);
            pointer->onWlPointerAxis(wlPointer, time, axis, value);
        },
        .frame = [](void* data, wl_pointer* wlPointer)
        {
            auto* pointer = reinterpret_cast<Pointer*>(data);
            pointer->onWlPointerFrame(wlPointer);
        },
        .axis_source = [](void* data, wl_pointer* wlPointer, uint32_t axis_source)
        {
            auto* pointer = reinterpret_cast<Pointer*>(data);
            pointer->onWlPointerAxisSource(wlPointer, axis_source);
        },
        .axis_stop = [](void* data, wl_pointer* wlPointer, uint32_t time, uint32_t axis_stop)
        {
            auto* pointer = reinterpret_cast<Pointer*>(data);
            pointer->onWlPointerAxisStop(wlPointer, time, axis_stop);
        },
        .axis_discrete = [](void* data, wl_pointer* wlPointer, uint32_t axis, int32_t discrete)
        {
            auto* pointer = reinterpret_cast<Pointer*>(data);
            pointer->onWlPointerAxisDiscrete(wlPointer, axis, discrete);
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

    void Pointer::onWlPointerEnter(wl_pointer* wlPointer, uint32_t serial, wl_surface* wlSurface, wl_fixed_t x, wl_fixed_t y)
    {
        _surfaceUnderPointer = reinterpret_cast<Surface*>(wl_surface_get_user_data(wlSurface));
    }

    void Pointer::onWlPointerLeave(wl_pointer* wlPointer, uint32_t serial, wl_surface* wlSurface)
    {
        _surfaceUnderPointer = nullptr;
    }

    void Pointer::onWlPointerMotion(wl_pointer* wlPointer, uint32_t time, wl_fixed_t x, wl_fixed_t y)
    {
        _surfaceUnderPointer->onPointerMotion();
    }

    void Pointer::onWlPointerButton(wl_pointer* wlPointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state)
    {
    }

    void Pointer::onWlPointerAxis(wl_pointer* wlPointer, uint32_t time, uint32_t axis, wl_fixed_t value)
    {
    }

    void Pointer::onWlPointerFrame(wl_pointer* wlPointer)
    {
    }

    void Pointer::onWlPointerAxisSource(wl_pointer* wlPointer, uint32_t axisSource)
    {
    }

    void Pointer::onWlPointerAxisStop(wl_pointer* wlPointer, uint32_t time, uint32_t axisStop)
    {
    }

    void Pointer::onWlPointerAxisDiscrete(wl_pointer* wlPointer, uint32_t axis, int32_t discrete)
    {
    }
}
