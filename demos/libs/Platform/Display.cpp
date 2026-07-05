#include "Display.h"

#include "Wayland/Display.h"

namespace Platform
{
    NonNull<StrongPtr<Display>> Display::_defaultDisplay = Wayland::Display::connect();
}
