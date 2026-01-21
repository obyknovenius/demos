#include <Core/EventLoop.h>
#include <Gfx/EGL/Context.h>
#include <Gfx/Wayland/Display.h>
#include <Gfx/Wayland/Toplevel.h>
#include <GLES3/gl3.h>

void draw(NonNull<RefPtr<Gfx::EGL::Context>> context)
{
    context->beginFrame();

    float red = rand() % 256 / 255.0f;
    float green = rand() % 256 / 255.0f;
    float blue = rand() % 256 / 255.0f;

    glClearColor(red, green, blue, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    context->endFrame();
}

int main(int argc, char** argv)
{
    auto display = Gfx::Wayland::Display::connect();
    if (!display)
        return 1;

    auto toplevel = Gfx::Wayland::Toplevel::create(display);

    auto context = Gfx::EGL::Context::create(toplevel);

    draw(context);

    toplevel->onPointerMotion = [&]() {
        draw(context);
    };

    Core::EventLoop::mainLoop()->run();

    return 0;
}
