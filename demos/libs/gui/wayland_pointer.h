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

    struct wl_pointer* m_wl_pointer {};

    wayland_seat* m_seat {};

    std::shared_ptr<wayland_window> m_window {};

    event m_event {};
};

}
