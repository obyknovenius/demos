#include "Window.h"

#include "Display.h"
#include "Wayland/Display.h"
#include "Wayland/Window.h"
#include <cassert>

namespace Platform
{
    NonNull<StrongPtr<Window>> Window::create(NonNull<StrongPtr<Display>> display, Gfx::Size size)
    {
        auto waylandDisplay = dynamicDowncast<Platform::Wayland::Display>(display.get());
        assert(waylandDisplay);
        return makeStrong<Wayland::Window>(waylandDisplay, size);
    }
}
