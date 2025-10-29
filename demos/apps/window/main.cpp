#include <gui/application.h>
#include <gui/display.h>
#include <gui/window.h>
#include <iostream>
#include <unistd.h>

int main()
{
    auto& app = gui::application::get();

    auto display = gui::display::connect();
    if (!display)
    {
        std::cerr << "Failed to connect to Wayland display." << std::endl;
        return 1;
    }

    auto window = display->create_window();
    window->on_close = [&app]()
    {
        app.quit();
    };

    app.run();

    return 0;
}
