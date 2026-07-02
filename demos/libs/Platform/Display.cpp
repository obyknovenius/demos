#include "Display.h"

#include "Wayland/Display.h"

namespace Platform
{
    StrongPtr<Display> Display::_defaultDisplay = Wayland::Display::connect();
}
