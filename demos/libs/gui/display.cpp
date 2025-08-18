#include "display.h"

#include "wayland_display.h"

namespace gui {

auto display::connect() -> std::shared_ptr<display>
{
    return wayland_display::connect();
}

}
