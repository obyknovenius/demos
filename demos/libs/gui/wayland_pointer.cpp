#include "wayland_pointer.h"

#include "event.h"
#include "wayland_display.h"
#include "wayland_window.h"
#include <iostream>

namespace gui {

wayland_pointer::wayland_pointer(wl_pointer* wl_pointer, wayland_seat* seat) :
    m_wl_pointer(wl_pointer),
    m_seat(seat)
{
    wl_pointer_add_listener(m_wl_pointer, &s_wl_pointer_listener, this);
}

wayland_pointer::~wayland_pointer()
{
    wl_pointer_release(m_wl_pointer);
}

const struct wl_pointer_listener wayland_pointer::s_wl_pointer_listener = {
    .enter = [](void* data, struct wl_pointer* wl_pointer, uint32_t serial, struct wl_surface* surface, wl_fixed_t x, wl_fixed_t y)
    {
        auto* pointer = reinterpret_cast<wayland_pointer*>(data);

        auto* window = reinterpret_cast<wayland_window*>(wl_surface_get_user_data(surface));
        pointer->m_window = window->shared_from_this();

        pointer->m_event = { event::type::invalid };
    },
    .leave = [](void* data, struct wl_pointer* wl_pointer, uint32_t serial, struct wl_surface* surface)
    {
        auto* pointer = reinterpret_cast<wayland_pointer*>(data);

        pointer->m_window.reset();

        pointer->m_event = { event::type::invalid };
    },
    .motion = [](void* data, struct wl_pointer* wl_pointer, uint32_t time, wl_fixed_t x, wl_fixed_t y)
    {
        auto* pointer = reinterpret_cast<wayland_pointer*>(data);

        pointer->m_event = { event::type::invalid };
    },
    .button = [](void* data, struct wl_pointer* wl_pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state)
    {
        auto* pointer = reinterpret_cast<wayland_pointer*>(data);

        pointer->m_event = {
            state == WL_POINTER_BUTTON_STATE_PRESSED ? event::type::button_pressed : event::type::button_released,
            pointer->m_window
        };
    },
    .axis = [](void* data, struct wl_pointer* wl_pointer, uint32_t time, uint32_t axis, wl_fixed_t value)
    {
        auto* pointer = reinterpret_cast<wayland_pointer*>(data);

        pointer->m_event = { event::type::invalid };
    },
    .frame = [](void* data, struct wl_pointer* wl_pointer)
    {
        auto* pointer = reinterpret_cast<wayland_pointer*>(data);

        pointer->m_seat->display()->dispatch_event(pointer->m_event);
    },
    .axis_source = [](void* data, struct wl_pointer* wl_pointer, uint32_t axis_source)
    {
        auto* pointer = reinterpret_cast<wayland_pointer*>(data);

        pointer->m_event = { event::type::invalid };
    },
    .axis_stop = [](void* data, struct wl_pointer* wl_pointer, uint32_t time, uint32_t axis_stop)
    {
        auto* pointer = reinterpret_cast<wayland_pointer*>(data);

        pointer->m_event = { event::type::invalid };
    },
    .axis_discrete = [](void* data, struct wl_pointer* wl_pointer, uint32_t axis, int32_t discrete)
    {
        auto* pointer = reinterpret_cast<wayland_pointer*>(data);

        pointer->m_event = { event::type::invalid };
    }
};

}
