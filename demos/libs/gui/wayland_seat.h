#pragma once

#include "wayland_pointer.h"
#include <core/ref_counted.h>
#include <core/weakable.h>
#include <wayland-client.h>

namespace gui {

class wayland_display;
class wayland_pointer;

class wayland_seat final : public weakable
{
    template<typename T, class... Args>
    friend nonnull_ref_ptr<T> core::make_ref_counted(Args&&...);

public:
    auto display() -> wayland_display* { return m_display; }

private:
    static const wl_seat_listener s_wl_seat_listener;

    wayland_seat(wl_seat* wl_seat, wayland_display* display);
    ~wayland_seat();

    auto on_capabilities(uint32_t capabilities) -> void;

    wl_seat* m_wl_seat {};

    wayland_display* m_display {};

    ref_ptr<wayland_pointer> m_pointer {};
};

}
