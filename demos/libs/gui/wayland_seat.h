#pragma once

#include <wayland-client.h>

namespace gui {

class wayland_display;
class wayland_pointer;

class wayland_seat
{
public:
    wayland_seat(wl_seat* wl_seat, wayland_display* display);
    ~wayland_seat();

    auto display() -> wayland_display* { return m_display; }

private:
    static const wl_seat_listener s_wl_seat_listener;

    auto on_capabilities(uint32_t capabilities) -> void;

    wl_seat* m_wl_seat {};

    wayland_display* m_display {};

    wayland_pointer* m_pointer {};
};

}
