#include "Window.h"

#include "Wayland/Window.h"

namespace Platform
{
    NonNull<StrongPtr<Window>> Window::window(Gfx::Size size)
    {
        return StrongPtr<Window>::adopt(new Wayland::Window(size));
    }
}
