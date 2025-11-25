#include "Pointer.h"

#include "Display.h"
#include "Seat.h"
#include "Window.h"
#include "../Cursor.h"
#include "../Event.h"

namespace GUI::Wayland
{
    Pointer::Pointer(wl_pointer* wlPointer, const NonnullRefPtr<Seat>& seat) :
        _wlPointer { wlPointer },
        _seat { seat }
    {
        wl_pointer_add_listener(_wlPointer, &_wlPointerListener, this);

        if (auto display = seat->display())
            _wpCursorShapeDeviceV1 = wp_cursor_shape_manager_v1_get_pointer(display->wpCursorShapeManagerV1(), _wlPointer);
    }

    Pointer::~Pointer()
    {
        wp_cursor_shape_device_v1_destroy(_wpCursorShapeDeviceV1);
        wl_pointer_release(_wlPointer);
    }

    const wl_pointer_listener Pointer::_wlPointerListener = {
        .enter = [](void* data, wl_pointer* wlPointer, uint32_t serial, wl_surface* surface, wl_fixed_t x, wl_fixed_t y)
        {
            auto* pointer = reinterpret_cast<Pointer*>(data);
            pointer->onEnter(serial, surface, x, y);
        },
        .leave = [](void* data, wl_pointer* wlPointer, uint32_t serial, wl_surface* surface)
        {
            auto* pointer = reinterpret_cast<Pointer*>(data);
            pointer->onLeave(serial, surface);
        },
        .motion = [](void* data, wl_pointer* wlPointer, uint32_t time, wl_fixed_t x, wl_fixed_t y)
        {
            auto* pointer = reinterpret_cast<Pointer*>(data);
            pointer->onMotion(time, x, y);
        },
        .button = [](void* data, wl_pointer* wlPointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state)
        {
            auto* pointer = reinterpret_cast<Pointer*>(data);
            pointer->onButton(serial, time, button, state);
        },
        .axis = [](void* data, wl_pointer* wlPointer, uint32_t time, uint32_t axis, wl_fixed_t value)
        {
            auto* pointer = reinterpret_cast<Pointer*>(data);
            pointer->onAxis(time, axis, value);
        },
        .frame = [](void* data, wl_pointer* wlPointer)
        {
            auto* pointer = reinterpret_cast<Pointer*>(data);
            pointer->onFrame();
        },
        .axis_source = [](void* data, wl_pointer* wlPointer, uint32_t axis_source)
        {
            auto* pointer = reinterpret_cast<Pointer*>(data);
            pointer->onAxisSource(axis_source);
        },
        .axis_stop = [](void* data, wl_pointer* wlPointer, uint32_t time, uint32_t axis_stop)
        {
            auto* pointer = reinterpret_cast<Pointer*>(data);
            pointer->onAxisStop(time, axis_stop);
        },
        .axis_discrete = [](void* data, wl_pointer* wlPointer, uint32_t axis, int32_t discrete)
        {
            auto* pointer = reinterpret_cast<Pointer*>(data);
            pointer->onAxisDiscrete(axis, discrete);
        }
    };

    void Pointer::onEnter(uint32_t serial, wl_surface* surface, wl_fixed_t x, wl_fixed_t y)
    {
        _window = reinterpret_cast<Window*>(wl_surface_get_user_data(surface));
        _position = { wl_fixed_to_int(x), wl_fixed_to_int(y) };
        _event = std::make_unique<Event>(Event::Type::PointerEntered, _position, _window);

        uint32_t shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_DEFAULT;
        switch (_window->currentCursor())
        {
            case Cursor::Default:
                shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_DEFAULT;
                break;
            case Cursor::Pointer:
                shape = WP_CURSOR_SHAPE_DEVICE_V1_SHAPE_POINTER;
                break;
        }
        wp_cursor_shape_device_v1_set_shape(_wpCursorShapeDeviceV1, serial, shape);
    }

    void Pointer::onLeave(uint32_t serial, wl_surface* surface)
    {
        _event = std::make_unique<Event>(Event::Type::PointerLeft, std::nullopt, _window);
        _window = nullptr;
    }

    void Pointer::onMotion(uint32_t time, wl_fixed_t x, wl_fixed_t y)
    {
        _position = { wl_fixed_to_int(x), wl_fixed_to_int(y) };
        _event = std::make_unique<Event>(Event::Type::PointerMoved, _position, _window);
    }

    void Pointer::onButton(uint32_t serial, uint32_t time, uint32_t button, uint32_t state)
    {
        if (auto seat = _seat.strong())
        {
            if (state == WL_POINTER_BUTTON_STATE_PRESSED)
            {
                if (_window->shouldStartMove(_position))
                {
                    xdg_toplevel_move(_window->xdgToplevel(), seat->wlSeat(), serial);
                    return;
                }
            }
        }

        auto type = state == WL_POINTER_BUTTON_STATE_PRESSED ? Event::Type::PointerButtonPressed : Event::Type::PointerButtonReleased;
        _event = std::make_unique<Event>(type, _position, _window);
    }

    void Pointer::onAxis(uint32_t time, uint32_t axis, wl_fixed_t value)
    {
    }

    void Pointer::onFrame()
    {
        if (_event)
            if (auto seat = _seat.strong())
                seat->display()->dispatchEvent(std::move(_event));

        _event.reset();
    }

    void Pointer::onAxisSource(uint32_t axis_source)
    {
    }

    void Pointer::onAxisStop(uint32_t time, uint32_t axis_stop)
    {
    }

    void Pointer::onAxisDiscrete(uint32_t axis, int32_t discrete)
    {
    }
}
