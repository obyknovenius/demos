#include <core/event_loop.h>
#include <gui/display.h>
#include <gui/window.h>
#include <iostream>
#include <unistd.h>

int main()
{
    auto display = gui::display::connect();
    if (!display)
    {
        std::cerr << "Failed to connect to Wayland display." << std::endl;
        return 1;
    }

    auto window = display->create_window();
    window->on_close = []()
    {
        core::event_loop::get_main().quit();
    };

    core::event_loop::get_main().run();

    return 0;
}
