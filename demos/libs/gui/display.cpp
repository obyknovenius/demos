#include "display.h"

#include "event.h"
#include "wayland/display.h"
#include "window.h"

namespace gui
{
    ref_ptr<display> display::connect()
    {
        return wayland::display::connect();
    }

    void display::dispatch_event(std::unique_ptr<const event> event)
    {
        auto window = event->window;
        if (window)
            window->dispatch_event(std::move(event));
    }
}
