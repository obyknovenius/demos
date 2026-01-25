#include <Core/EventLoop.h>
#include <Gfx/Wayland/Display.h>
#include <Gfx/Wayland/Toplevel.h>
#include <GLES3/gl3.h>

static bool drawing = false;

class ToplevelDelegate : public Gfx::Wayland::Surface::Delegate
{
public:
    void surfaceDidFinishFrame(RefPtr<Gfx::Wayland::Surface> surface) override
    {
        drawing = false;
    }
};

void draw(NonNull<RefPtr<Gfx::Wayland::Toplevel>> toplevel)
{
    if (drawing)
        return;

    drawing = true;

    toplevel->beginFrame();

    float red = rand() % 256 / 255.0f;
    float green = rand() % 256 / 255.0f;
    float blue = rand() % 256 / 255.0f;

    glClearColor(red, green, blue, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    toplevel->endFrame();
}

int main(int argc, char** argv)
{
    auto display = Gfx::Wayland::Display::connect();
    if (!display)
        return 1;

    auto toplevel = Gfx::Wayland::Toplevel::create(display);

    auto delegate = adopt(new ToplevelDelegate());
    toplevel->setDelegate(delegate);

    draw(toplevel);

    toplevel->onPointerMotion = [&]() {
        draw(toplevel);
    };

    Core::EventLoop::mainLoop()->run();

    return 0;
}
