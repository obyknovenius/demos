#include <Core/EventLoop.h>
#include <Platform/Display.h>
#include <Platform/Window.h>

int main(int argc, char** argv)
{
    auto display = Platform::Display::defaultDisplay();
    auto window = Platform::Window::create(display);

    Core::EventLoop::mainLoop()->run();

    return 0;
}
