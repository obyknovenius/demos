#include "Window.h"

#include "Display.h"
#include "Wayland/Display.h"
#include "Wayland/Window.h"

namespace Platform
{
    NonNullRefPtr<Window> Window::create(NonNullRefPtr<Display> display, Gfx::Size size)
    {
        auto waylandDisplay = staticPtrCast<Platform::Wayland::Display>(display);
        return Wayland::Window::create(waylandDisplay, size);
    }
}
