#include "Pointer.h"

#include "Seat.h"
#include "Window.h"

namespace Platform::Wayland
{
    NonNull<RefPtr<Pointer>> Pointer::create(NonNull<wl_pointer*> wlPointer, NonNull<RefPtr<Seat>> seat)
    {
        return adopt(new Pointer(wlPointer, seat));
    }

    const wl_pointer_listener Pointer::_wlPointerListener = {
        .enter = [](void* data, wl_pointer* wlPointer, uint32_t serial, wl_surface* surface, wl_fixed_t x, wl_fixed_t y)
        {
            auto* pointer = reinterpret_cast<Pointer*>(data);
            auto* window = reinterpret_cast<Window*>(wl_surface_get_user_data(surface));
            pointer->didEnterWindow(serial, window, wl_fixed_to_double(x), wl_fixed_to_double(y));
        },
        .leave = [](void* data, wl_pointer* wlPointer, uint32_t serial, wl_surface* surface)
        {
            auto* pointer = reinterpret_cast<Pointer*>(data);
            auto* window = reinterpret_cast<Window*>(wl_surface_get_user_data(surface));
            pointer->didLeaveWindow(serial, window);
        },
        .motion = [](void* data, wl_pointer* wlPointer, uint32_t time, wl_fixed_t x, wl_fixed_t y)
        {
            auto* pointer = reinterpret_cast<Pointer*>(data);
            pointer->didMove(time, wl_fixed_to_double(x), wl_fixed_to_double(y));
        },
        .button = [](void* data, wl_pointer* wlPointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state)
        {
        },
        .axis = [](void* data, wl_pointer* wlPointer, uint32_t time, uint32_t axis, wl_fixed_t value)
        {
        },
        .frame = [](void* data, wl_pointer* wlPointer)
        {
            auto* pointer = reinterpret_cast<Pointer*>(data);
            pointer->sendEvent();
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

    void Pointer::didEnterWindow(uint32_t serial, NonNull<RefPtr<Window>> window, double x, double y)
    {
        _event = { Event::Type::PointerEntered };
        _window = window;
    }

    void Pointer::didLeaveWindow(uint32_t serial, NonNull<RefPtr<Window>> window)
    {
        _event = { Event::Type::PointerLeft };
        _window = nullptr;
    }

    void Pointer::didMove(uint32_t time, double x, double y)
    {
        _event = { Event::Type::PointerMoved };
    }

    void Pointer::sendEvent()
    {
        if (auto window = _window.strong())
        {
            window->receiveEvent(*_event);
            _event.reset();
        }
    }
}
