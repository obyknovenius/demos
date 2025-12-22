#pragma once

#include "../Cursor.h"
#include "../Event.h"
#include <Core/RefCounted.h>
#include <Core/WeakPtr.h>
#include <Gfx/Point.h>
#include <memory>
#include <wayland-client.h>
#include <cursor-shape-v1-client-protocol.h>

namespace GUI::Wayland
{
    class Seat;
    class Window;

    class Pointer final : public RefCounted
    {
    public:
        static RefPtr<Pointer> make(wl_pointer* wlPointer, const RefPtr<Seat>& seat);

        void setCursor(Cursor cursor);

        uint32_t lastSerial() const;

    private:
        static const wl_pointer_listener _wlPointerListener;

        Pointer(wl_pointer* wlPointer, const RefPtr<Seat>& seat);
        ~Pointer();

        void onEnter(uint32_t serial, wl_surface* surface, wl_fixed_t x, wl_fixed_t y);
        void onLeave(uint32_t serial, wl_surface* surface);
        void onMotion(uint32_t time, wl_fixed_t x, wl_fixed_t y);
        void onButton(uint32_t serial, uint32_t time, uint32_t button, uint32_t state);
        void onAxis(uint32_t time, uint32_t axis, wl_fixed_t value);
        void onFrame();
        void onAxisSource(uint32_t axisSource);
        void onAxisStop(uint32_t time, uint32_t axisStop);
        void onAxisDiscrete(uint32_t axis, int32_t discrete);

        wl_pointer* _wlPointer {};
        wp_cursor_shape_device_v1* _wpCursorShapeDeviceV1 {};

        WeakPtr<Seat> _seat;

        RefPtr<Window> _window;
        Gfx::Point _position {};
        Event::PointerButtons _pressedButtons {};

        std::unique_ptr<Event> _event {};

        uint32_t _lastSerial { 0 };
        uint32_t _lastEnterSerial { 0 };
    };

    inline RefPtr<Pointer> Pointer::make(wl_pointer* wlPointer, const RefPtr<Seat>& seat)
    {
        return adopt(new Pointer(wlPointer, seat));
    }

    inline uint32_t Pointer::lastSerial() const
    {
        return _lastSerial;
    }
}
