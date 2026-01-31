#include "Window.h"

#include "Display.h"
#include "Wayland/Display.h"
#include "Wayland/Window.h"

namespace Platform
{
    NonNull<RefPtr<Window>> Window::create(NonNull<RefPtr<Display>> display, Gfx::Size size)
    {
        return Wayland::Window::create(Display::defaultDisplay(), size);
    }
}
