#pragma once

#include <Core/RefCounted.h>
#include <Core/WeakPtr.h>
#include <Gfx/Point.h>
#include <memory>
#include <wayland-client.h>

namespace gui
{
    struct event;
}

namespace gui::wayland
{
    class cursor;
    class seat;
    class window;

    class pointer final : public RefCounted
    {
    public:
        static NonnullRefPtr<pointer> make(wl_pointer* wl_pointer, const NonnullRefPtr<seat>& seat);

    private:
        static const wl_pointer_listener s_wl_pointer_listener;

        pointer(wl_pointer* wl_pointer, const NonnullRefPtr<seat>& seat);
        ~pointer();

        void on_enter(uint32_t serial, wl_surface* surface, wl_fixed_t x, wl_fixed_t y);
        void on_leave(uint32_t serial, wl_surface* surface);
        void on_motion(uint32_t time, wl_fixed_t x, wl_fixed_t y);
        void on_button(uint32_t serial, uint32_t time, uint32_t button, uint32_t state);
        void on_axis(uint32_t time, uint32_t axis, wl_fixed_t value);
        void on_frame();
        void on_axis_source(uint32_t axis_source);
        void on_axis_stop(uint32_t time, uint32_t axis_stop);
        void on_axis_discrete(uint32_t axis, int32_t discrete);

        wl_pointer* m_wl_pointer {};

        WeakPtr<seat> m_seat;
        std::unique_ptr<cursor> m_cursor;

        RefPtr<window> m_window;
        Gfx::Point m_position;

        std::unique_ptr<event> m_event;
    };

    inline NonnullRefPtr<pointer> pointer::make(wl_pointer* wl_pointer, const NonnullRefPtr<seat>& seat)
    {
        return adopt(*new pointer(wl_pointer, seat));
    }
}
