#include "window.h"

namespace gui {

auto window::dispatch_event(const event event) -> void
{
    close();
}

auto window::close() -> void
{
    if (on_close)
        on_close();
}

}
