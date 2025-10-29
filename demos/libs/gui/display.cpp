#include "display.h"

#include "event.h"
#include "wayland/display.h"
#include "window.h"
#include <mutex>

namespace gui
{
    ref_ptr<display> display::s_default = nullptr;

    ref_ptr<display> display::get_default()
    {
        static std::once_flag once;
        std::call_once(once, []
        {
            if (!s_default)
                s_default = wayland::display::connect();
        });
        return s_default;
    }

    void display::dispatch_event(std::unique_ptr<const event> event)
    {
        auto window = event->window;
        if (window)
            window->dispatch_event(std::move(event));
    }
}
