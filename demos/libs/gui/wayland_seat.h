#pragma once

#include <wayland-client.h>

namespace gui {

class wayland_display;
class wayland_pointer;

class wayland_seat
{
public:
    wayland_seat(struct wl_seat* wl_seat, wayland_display* display);
    ~wayland_seat();

    auto display() -> wayland_display* { return m_display; }

private:
    static const struct wl_seat_listener s_wl_seat_listener;
    static const struct wl_pointer_listener s_wl_pointer_listener;

    struct wl_seat* m_wl_seat {};

    wayland_display* m_display {};

    wayland_pointer* m_pointer {};
};

}
