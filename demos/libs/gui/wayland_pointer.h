#pragma once

#include "event.h"
#include "wayland_window.h"
#include "wayland_seat.h"
#include <core/ref_counted.h>
#include <core/weak_ptr.h>
#include <optional>
#include <wayland-client.h>

namespace gui {

class wayland_seat;

class wayland_pointer final : public ref_counted
{
private:
    static const wl_pointer_listener s_wl_pointer_listener;

    wayland_pointer(wl_pointer* wl_pointer, nonnull_ref_ptr<wayland_seat> seat);
    ~wayland_pointer();

    auto on_enter(uint32_t serial, wl_surface* surface, wl_fixed_t x, wl_fixed_t y) -> void;
    auto on_leave(uint32_t serial, wl_surface* surface) -> void;
    auto on_motion(uint32_t time, wl_fixed_t x, wl_fixed_t y) -> void;
    auto on_button(uint32_t serial, uint32_t time, uint32_t button, uint32_t state) -> void;
    auto on_axis(uint32_t time, uint32_t axis, wl_fixed_t value) -> void;
    auto on_frame() -> void;
    auto on_axis_source(uint32_t axis_source) -> void;
    auto on_axis_stop(uint32_t time, uint32_t axis_stop) -> void;
    auto on_axis_discrete(uint32_t axis, int32_t discrete) -> void;

    wl_pointer* m_wl_pointer {};

    weak_ptr<wayland_seat> m_seat;

    ref_ptr<wayland_window> m_window {};
    std::optional<event> m_event {};

    template<typename T, class... Args>
    friend nonnull_ref_ptr<T> core::make_ref_counted(Args&&...);
};

}
