#include "Display.h"

#include "Wayland/Display.h"

namespace Platform
{
    RefPtr<Display> Display::_defaultDisplay = Wayland::Display::connect();
}
