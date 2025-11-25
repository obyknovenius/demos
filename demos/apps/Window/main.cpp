#include <GUI/Application.h>
#include <GUI/Cursor.h>
#include <GUI/Window.h>

int main()
{
    auto& app = GUI::Application::sharedApplication();

    auto window = GUI::Window::make();
    window->setCursor(GUI::Cursor::Pointer);
    window->onClose = [&app]()
    {
        app.quit();
    };

    app.run();

    return 0;
}
