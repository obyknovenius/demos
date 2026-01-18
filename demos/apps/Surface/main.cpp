#include <Core/EventLoop.h>
#include <Gfx/EGL/Context.h>
#include <Gfx/Wayland/Display.h>
#include <Gfx/Wayland/Toplevel.h>
#include <GLES3/gl3.h>

int main(int argc, char** argv)
{
    auto display = Gfx::Wayland::Display::connect();
    if (!display)
        return 1;

    auto toplevel = Gfx::Wayland::Toplevel::create(display);

    auto context = Gfx::EGL::Context::create(toplevel);

    context->beginFrame();

    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    context->endFrame();

    Core::EventLoop::mainLoop()->run();

    return 0;
}
