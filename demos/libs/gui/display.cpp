#include "display.h"

#include "wayland_display.h"
#include "window.h"

namespace gui {

auto display::connect() -> std::shared_ptr<display>
{
    return wayland_display::connect();
}

auto display::dispatch_event(const event event) -> void
{
    auto window = event.window;
    if (window)
        window->dispatch_event(event);
}

}
