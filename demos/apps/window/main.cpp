#include <gui/application.h>
#include <gui/display.h>
#include <gui/window.h>

int main()
{
    auto& app = gui::application::get();

    auto window = gui::display::get_default()->create_window();
    window->on_close = [&app]()
    {
        app.quit();
    };

    app.run();

    return 0;
}
