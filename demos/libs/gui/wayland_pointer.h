#pragma once

#include "event.h"
#include "wayland_seat.h"
#include <wayland-client.h>

namespace gui {

class wayland_seat;
class wayland_window;

class wayland_pointer
{
public:
    wayland_pointer(struct wl_pointer* wl_pointer, wayland_seat* seat);
    ~wayland_pointer();

private:
    static const struct wl_pointer_listener s_wl_pointer_listener;

    auto on_enter(uint32_t serial, struct wl_surface* surface, wl_fixed_t x, wl_fixed_t y) -> void;
    auto on_leave(uint32_t serial, struct wl_surface* surface) -> void;
    auto on_motion(uint32_t time, wl_fixed_t x, wl_fixed_t y) -> void;
    auto on_button(uint32_t serial, uint32_t time, uint32_t button, uint32_t state) -> void;
    auto on_axis(uint32_t time, uint32_t axis, wl_fixed_t value) -> void;
    auto on_frame() -> void;
    auto on_axis_source(uint32_t axis_source) -> void;
    auto on_axis_stop(uint32_t time, uint32_t axis_stop) -> void;
    auto on_axis_discrete(uint32_t axis, int32_t discrete) -> void;

    wl_pointer* m_wl_pointer {};

    wayland_seat* m_seat {};

    std::shared_ptr<wayland_window> m_window {};

    event m_event {};
};

}
