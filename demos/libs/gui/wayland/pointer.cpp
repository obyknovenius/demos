#include "pointer.h"

#include "cursor.h"
#include "event.h"
#include "display.h"
#include "seat.h"
#include "window.h"

namespace gui::wayland
{
    pointer::pointer(wl_pointer* wl_pointer, const nonnull_ref_ptr<seat>& seat) :
        m_wl_pointer { wl_pointer },
        m_seat { seat },
        m_cursor { std::make_unique<cursor>(seat->get_display()) }
    {
        wl_pointer_add_listener(m_wl_pointer, &s_wl_pointer_listener, this);
    }

    pointer::~pointer()
    {
        wl_pointer_release(m_wl_pointer);
    }

    const wl_pointer_listener pointer::s_wl_pointer_listener = {
        .enter = [](void* data, wl_pointer* wl_pointer, uint32_t serial, wl_surface* surface, wl_fixed_t x, wl_fixed_t y)
        {
            auto* pointer = reinterpret_cast<class pointer*>(data);
            pointer->on_enter(serial, surface, x, y);
        },
        .leave = [](void* data, wl_pointer* wl_pointer, uint32_t serial, wl_surface* surface)
        {
            auto* pointer = reinterpret_cast<class pointer*>(data);
            pointer->on_leave(serial, surface);
        },
        .motion = [](void* data, wl_pointer* wl_pointer, uint32_t time, wl_fixed_t x, wl_fixed_t y)
        {
            auto* pointer = reinterpret_cast<class pointer*>(data);
            pointer->on_motion(time, x, y);
        },
        .button = [](void* data, wl_pointer* wl_pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state)
        {
            auto* pointer = reinterpret_cast<class pointer*>(data);
            pointer->on_button(serial, time, button, state);
        },
        .axis = [](void* data, wl_pointer* wl_pointer, uint32_t time, uint32_t axis, wl_fixed_t value)
        {
            auto* pointer = reinterpret_cast<class pointer*>(data);
            pointer->on_axis(time, axis, value);
        },
        .frame = [](void* data, wl_pointer* wl_pointer)
        {
            auto* pointer = reinterpret_cast<class pointer*>(data);
            pointer->on_frame();
        },
        .axis_source = [](void* data, wl_pointer* wl_pointer, uint32_t axis_source)
        {
            auto* pointer = reinterpret_cast<class pointer*>(data);
            pointer->on_axis_source(axis_source);
        },
        .axis_stop = [](void* data, wl_pointer* wl_pointer, uint32_t time, uint32_t axis_stop)
        {
            auto* pointer = reinterpret_cast<class pointer*>(data);
            pointer->on_axis_stop(time, axis_stop);
        },
        .axis_discrete = [](void* data, wl_pointer* wl_pointer, uint32_t axis, int32_t discrete)
        {
            auto* pointer = reinterpret_cast<class pointer*>(data);
            pointer->on_axis_discrete(axis, discrete);
        }
    };

    void pointer::on_enter(uint32_t serial, wl_surface* surface, wl_fixed_t x, wl_fixed_t y)
    {
        wl_pointer_set_cursor(m_wl_pointer, serial, m_cursor->get_wl_surface(), m_cursor->get_wl_cursor_image()->hotspot_x, m_cursor->get_wl_cursor_image()->hotspot_y);

        m_window = reinterpret_cast<window*>(wl_surface_get_user_data(surface));
        m_position = { wl_fixed_to_int(x), wl_fixed_to_int(y) };
    }

    void pointer::on_leave(uint32_t serial, wl_surface* surface)
    {
        m_window = nullptr;
    }

    void pointer::on_motion(uint32_t time, wl_fixed_t x, wl_fixed_t y)
    {
        m_position = { wl_fixed_to_int(x), wl_fixed_to_int(y) };
    }

    void pointer::on_button(uint32_t serial, uint32_t time, uint32_t button, uint32_t state)
    {
        if (auto seat = m_seat.strong_ref())
        {
            if (state == WL_POINTER_BUTTON_STATE_PRESSED)
            {
                if (m_window->should_start_move(m_position))
                {
                    xdg_toplevel_move(m_window->get_xdg_toplevel(), seat->get_wl_seat(), serial);
                    return;
                }
            }
        }

        auto type = state == WL_POINTER_BUTTON_STATE_PRESSED ? event::type::button_pressed : event::type::button_released;
        m_event = std::make_unique<event>(type, m_window);
    }

    void pointer::on_axis(uint32_t time, uint32_t axis, wl_fixed_t value)
    {
    }

    void pointer::on_frame()
    {
        if (m_event)
            if (auto seat = m_seat.strong_ref())
                seat->get_display()->dispatch_event(std::move(m_event));

        m_event.reset();
    }

    void pointer::on_axis_source(uint32_t axis_source)
    {
    }

    void pointer::on_axis_stop(uint32_t time, uint32_t axis_stop)
    {
    }

    void pointer::on_axis_discrete(uint32_t axis, int32_t discrete)
    {
    }
}
