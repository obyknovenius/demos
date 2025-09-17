#include "wayland_pointer.h"

#include "wayland_display.h"
#include "wayland_seat.h"
#include "wayland_window.h"

namespace gui {

wayland_pointer::wayland_pointer(wl_pointer* wl_pointer, nonnull_ref_ptr<wayland_seat> seat) :
    m_wl_pointer { wl_pointer },
    m_seat { seat }
{
    wl_pointer_add_listener(m_wl_pointer, &s_wl_pointer_listener, this);
}

wayland_pointer::~wayland_pointer()
{
    wl_pointer_release(m_wl_pointer);
}

const wl_pointer_listener wayland_pointer::s_wl_pointer_listener = {
    .enter = [](void* data, wl_pointer* wl_pointer, uint32_t serial, wl_surface* surface, wl_fixed_t x, wl_fixed_t y)
    {
        auto* pointer = reinterpret_cast<wayland_pointer*>(data);
        pointer->on_enter(serial, surface, x, y);
    },
    .leave = [](void* data, wl_pointer* wl_pointer, uint32_t serial, wl_surface* surface)
    {
        auto* pointer = reinterpret_cast<wayland_pointer*>(data);
        pointer->on_leave(serial, surface);
    },
    .motion = [](void* data, wl_pointer* wl_pointer, uint32_t time, wl_fixed_t x, wl_fixed_t y)
    {
        auto* pointer = reinterpret_cast<wayland_pointer*>(data);
        pointer->on_motion(time, x, y);
    },
    .button = [](void* data, wl_pointer* wl_pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state)
    {
        auto* pointer = reinterpret_cast<wayland_pointer*>(data);
        pointer->on_button(serial, time, button, state);
    },
    .axis = [](void* data, wl_pointer* wl_pointer, uint32_t time, uint32_t axis, wl_fixed_t value)
    {
        auto* pointer = reinterpret_cast<wayland_pointer*>(data);
        pointer->on_axis(time, axis, value);
    },
    .frame = [](void* data, wl_pointer* wl_pointer)
    {
        auto* pointer = reinterpret_cast<wayland_pointer*>(data);
        pointer->on_frame();
    },
    .axis_source = [](void* data, wl_pointer* wl_pointer, uint32_t axis_source)
    {
        auto* pointer = reinterpret_cast<wayland_pointer*>(data);
        pointer->on_axis_source(axis_source);
    },
    .axis_stop = [](void* data, wl_pointer* wl_pointer, uint32_t time, uint32_t axis_stop)
    {
        auto* pointer = reinterpret_cast<wayland_pointer*>(data);
        pointer->on_axis_stop(time, axis_stop);
    },
    .axis_discrete = [](void* data, wl_pointer* wl_pointer, uint32_t axis, int32_t discrete)
    {
        auto* pointer = reinterpret_cast<wayland_pointer*>(data);
        pointer->on_axis_discrete(axis, discrete);
    }
};

auto wayland_pointer::on_enter(uint32_t serial, wl_surface* surface, wl_fixed_t x, wl_fixed_t y) -> void
{
    m_window = reinterpret_cast<wayland_window*>(wl_surface_get_user_data(surface));

    m_event = { event::type::invalid };
}

auto wayland_pointer::on_leave(uint32_t serial, wl_surface* surface) -> void
{
    m_window = nullptr;

    m_event = { event::type::invalid };
}

auto wayland_pointer::on_motion(uint32_t time, wl_fixed_t x, wl_fixed_t y) -> void
{
    m_event = { event::type::invalid };
}

auto wayland_pointer::on_button(uint32_t serial, uint32_t time, uint32_t button, uint32_t state) -> void
{
    m_event = {
        state == WL_POINTER_BUTTON_STATE_PRESSED ? event::type::button_pressed : event::type::button_released,
        m_window
    };
}

auto wayland_pointer::on_axis(uint32_t time, uint32_t axis, wl_fixed_t value) -> void
{
    m_event = { event::type::invalid };
}

auto wayland_pointer::on_frame() -> void
{
    if (m_event)
        if (auto seat = m_seat.strong_ref())
            seat->display()->dispatch_event(*m_event);

    m_event.reset();
}

auto wayland_pointer::on_axis_source(uint32_t axis_source) -> void
{
    m_event = { event::type::invalid };
}

auto wayland_pointer::on_axis_stop(uint32_t time, uint32_t axis_stop) -> void
{
    m_event = { event::type::invalid };
}

auto wayland_pointer::on_axis_discrete(uint32_t axis, int32_t discrete) -> void
{
    m_event = { event::type::invalid };
}


}
