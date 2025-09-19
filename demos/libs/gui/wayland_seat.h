#pragma once

#include <core/weakable.h>
#include <core/weak_ptr.h>
#include <wayland-client.h>

namespace gui {

class wayland_display;
class wayland_pointer;

class wayland_seat final : public weakable
{
    template<typename T, class... Args>
    friend nonnull_ref_ptr<T> core::make_ref_counted(Args&&...);

    friend class wayland_pointer;

public:
    auto display() -> ref_ptr<wayland_display> { return m_display.strong_ref(); }

private:
    static const wl_seat_listener s_wl_seat_listener;

    wayland_seat(wl_seat* wl_seat, nonnull_ref_ptr<wayland_display> display);
    ~wayland_seat();

    auto on_capabilities(uint32_t capabilities) -> void;

    wl_seat* m_wl_seat {};

    weak_ptr<wayland_display> m_display;

    ref_ptr<wayland_pointer> m_pointer;
};

}
