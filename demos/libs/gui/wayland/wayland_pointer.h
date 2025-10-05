#pragma once

#include "wayland_window.h"
#include <core/ref_counted.h>
#include <core/weak_ptr.h>
#include <memory>
#include <wayland-client.h>

namespace gui
{
    class wayland_seat;
    struct event;

    class wayland_pointer final : public ref_counted
    {
    public:
        static nonnull_ref_ptr<wayland_pointer> create(wl_pointer* wl_pointer, const nonnull_ref_ptr<wayland_seat>& seat)
        {
            return adopt(*new wayland_pointer(wl_pointer, seat));
        }

    private:
        static const wl_pointer_listener s_wl_pointer_listener;

        wayland_pointer(wl_pointer* wl_pointer, const nonnull_ref_ptr<wayland_seat>& seat);
        ~wayland_pointer();

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

        weak_ptr<wayland_seat> m_seat;

        ref_ptr<wayland_window> m_window;
        std::unique_ptr<event> m_event;
    };
}
