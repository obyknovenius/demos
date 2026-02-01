#include <Core/EventLoop.h>
#include <Platform/Display.h>
#include <Platform/Window.h>
#include <GLES3/gl3.h>
#include <cstdlib>

class WindowDelegate final : public Platform::Window::Delegate
{
public:
    static NonNull<RefPtr<WindowDelegate>> create()
    {
        return adopt(new WindowDelegate());
    }

    void draw(NonNull<RefPtr<Platform::Window>> window) override
    {
        float red = rand() % 256 / 255.0f;
        float green = rand() % 256 / 255.0f;
        float blue = rand() % 256 / 255.0f;

        glClearColor(red, green, blue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

private:
    WindowDelegate() = default;
};

int main(int argc, char** argv)
{
    auto display = Platform::Display::defaultDisplay();
    auto window = Platform::Window::create(display);

    auto windowDelegate = WindowDelegate::create();
    window->setDelegate(windowDelegate);

    Core::EventLoop::mainLoop()->run();

    return 0;
}
