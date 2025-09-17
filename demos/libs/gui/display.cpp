#include "display.h"

#include "event.h"
#include "wayland_display.h"
#include "window.h"

namespace gui {

auto display::connect() -> ref_ptr<display>
{
    return wayland_display::connect();
}

auto display::dispatch_event(std::unique_ptr<const event> event) -> void
{
    auto window = event->window;
    if (window)
        window->dispatch_event(std::move(event));
}

}
