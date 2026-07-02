#include "Window.h"

#include "Display.h"
#include "Wayland/Display.h"
#include "Wayland/Window.h"
#include <cassert>

namespace Platform
{
    StrongPtr<Window> Window::create(StrongPtr<Display> display, Gfx::Size size)
    {
        auto waylandDisplay = dynamicDowncast<Platform::Wayland::Display>(display);
        assert(waylandDisplay);
        return makeStrong<Wayland::Window>(waylandDisplay, size);
    }
}
