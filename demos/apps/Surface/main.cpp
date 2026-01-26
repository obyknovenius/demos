#include <Core/EventLoop.h>
#include <Gfx/Wayland/Display.h>
#include <Gfx/Wayland/Toplevel.h>
#include <GLES3/gl3.h>

int main(int argc, char** argv)
{
    auto display = Gfx::Wayland::Display::connect();
    if (!display)
        return 1;

    auto toplevel = Gfx::Wayland::Toplevel::create(display);

    toplevel->onPointerMotion = [&]() {
        toplevel->setNeedsRedraw();
    };

    Core::EventLoop::mainLoop()->run();

    return 0;
}
