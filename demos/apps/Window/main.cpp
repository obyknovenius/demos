#include <GUI/Application.h>
#include <GUI/Cursor.h>
#include <GUI/Window.h>

int main()
{
    auto& app = GUI::Application::sharedApplication();

    GUI::Cursor::set(GUI::Cursor::pointerCursor());

    auto window = GUI::Window::make();
    window->onClose = [&app]()
    {
        app.quit();
    };

    app.run();

    return 0;
}
