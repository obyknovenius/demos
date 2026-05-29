#include "Window.h"

#include "Display.h"
#include "Wayland/Display.h"
#include "Wayland/Window.h"

namespace Platform
{
    NonNull<RefPtr<Window>> Window::create(NonNull<RefPtr<Display>> display, Gfx::Size size)
    {
        auto waylandDisplay = downcast<Platform::Wayland::Display>(display);
        return Wayland::Window::create(waylandDisplay, size);
    }
}
