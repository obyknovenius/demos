#include <GUI/Application.h>
#include <GUI/Window.h>

int main()
{
    auto& app = GUI::Application::sharedApplication();

    auto window = GUI::Window::make();
    window->onClose = [&app]()
    {
        app.quit();
    };

    app.run();

    return 0;
}
