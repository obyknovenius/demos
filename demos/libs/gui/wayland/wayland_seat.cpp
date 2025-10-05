#include "wayland_seat.h"

#include "wayland_display.h"
#include "wayland_pointer.h"

namespace gui {

const wl_seat_listener wayland_seat::s_wl_seat_listener = {
    .capabilities = [](void* data, wl_seat* wl_seat, uint32_t capabilities)
    {
        auto* seat = reinterpret_cast<wayland_seat*>(data);
        seat->on_capabilities(capabilities);
    },
    .name = [](void* data, wl_seat* wl_seat, const char* name)
    {
    }
};

wayland_seat::wayland_seat(wl_seat* wl_seat, const nonnull_ref_ptr<wayland_display>& display) :
    m_wl_seat { wl_seat },
    m_display { display }
{
    wl_seat_add_listener(m_wl_seat, &s_wl_seat_listener, this);
}

wayland_seat::~wayland_seat()
{
    m_pointer = nullptr;

    wl_seat_release(m_wl_seat);
}

auto wayland_seat::on_capabilities(uint32_t capabilities) -> void
{
    bool have_pointer = capabilities & WL_SEAT_CAPABILITY_POINTER;

    if (have_pointer && !m_pointer)
        m_pointer = wayland_pointer::create(wl_seat_get_pointer(m_wl_seat), *this);
    else if (!have_pointer && m_pointer)
        m_pointer = nullptr;
}

}
