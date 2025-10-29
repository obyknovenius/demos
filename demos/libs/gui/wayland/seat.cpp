#include "seat.h"

#include "display.h"
#include "pointer.h"

namespace gui::wayland
{
    const wl_seat_listener seat::s_wl_seat_listener = {
        .capabilities = [](void* data, wl_seat* wl_seat, uint32_t capabilities)
        {
            auto* seat = reinterpret_cast<class seat*>(data);
            seat->on_capabilities(capabilities);
        },
        .name = [](void* data, wl_seat* wl_seat, const char* name)
        {
        }
    };

    seat::seat(wl_seat* wl_seat, const nonnull_ref_ptr<display>& display) :
        m_wl_seat { wl_seat },
        m_display { display }
    {
        wl_seat_add_listener(m_wl_seat, &s_wl_seat_listener, this);
    }

    seat::~seat()
    {
        m_pointer = nullptr;

        wl_seat_release(m_wl_seat);
    }

    void seat::on_capabilities(uint32_t capabilities)
    {
        bool have_pointer = capabilities & WL_SEAT_CAPABILITY_POINTER;

        if (have_pointer && !m_pointer)
            m_pointer = pointer::make(wl_seat_get_pointer(m_wl_seat), *this);
        else if (!have_pointer && m_pointer)
            m_pointer = nullptr;
    }
}
