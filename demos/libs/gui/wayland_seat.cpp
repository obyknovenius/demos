#include "wayland_seat.h"

#include "wayland_display.h"
#include "wayland_pointer.h"

namespace gui {

const struct wl_seat_listener wayland_seat::s_wl_seat_listener = {
    .capabilities = [](void* data, struct wl_seat* wl_seat, uint32_t capabilities)
    {
        auto* seat = reinterpret_cast<wayland_seat*>(data);
        
        bool have_pointer = capabilities & WL_SEAT_CAPABILITY_POINTER;

        if (have_pointer && !seat->m_pointer)
            seat->m_pointer = new wayland_pointer(wl_seat_get_pointer(wl_seat), seat);
        else if (!have_pointer && seat->m_pointer)
        {
            delete seat->m_pointer;
            seat->m_pointer = nullptr;
        }
    },
    .name = [](void* data, struct wl_seat* wl_seat, const char* name)
    {
    }
};

wayland_seat::wayland_seat(struct wl_seat* wl_seat, wayland_display* display) :
    m_wl_seat(wl_seat),
    m_display(display)
{
    wl_seat_add_listener(m_wl_seat, &s_wl_seat_listener, this);
}

wayland_seat::~wayland_seat()
{
    wl_seat_release(m_wl_seat);
}

}
