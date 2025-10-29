#include <gui/application.h>
#include <gui/window.h>

int main()
{
    auto& app = gui::application::get();

    auto window = gui::window::create();
    window->on_close = [&app]()
    {
        app.quit();
    };

    app.run();

    return 0;
}
