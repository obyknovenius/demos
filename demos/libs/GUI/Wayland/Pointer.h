#pragma once

#include <Core/RefCounted.h>
#include <Core/WeakPtr.h>
#include <Gfx/Point.h>
#include <memory>
#include <wayland-client.h>

namespace GUI
{
    struct Event;
}

namespace GUI::Wayland
{
    class Cursor;
    class Seat;
    class Window;

    class Pointer final : public RefCounted
    {
    public:
        static NonnullRefPtr<Pointer> make(wl_pointer* wlPointer, const NonnullRefPtr<Seat>& seat);

    private:
        static const wl_pointer_listener _wlPointerListener;

        Pointer(wl_pointer* wlPointer, const NonnullRefPtr<Seat>& seat);
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

        WeakPtr<Seat> _seat;
        std::unique_ptr<Cursor> _cursor;

        RefPtr<Window> _window;
        Gfx::Point _position;

        std::unique_ptr<Event> _event;
    };

    inline NonnullRefPtr<Pointer> Pointer::make(wl_pointer* wlPointer, const NonnullRefPtr<Seat>& seat)
    {
        return adopt(*new Pointer(wlPointer, seat));
    }
}
