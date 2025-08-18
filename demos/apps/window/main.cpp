#include <core/event_loop.h>
#include <gui/display.h>
#include <iostream>
#include <unistd.h>

auto main() -> int
{
    auto display = gui::display::connect();
    if (!display)
    {
        std::cerr << "Failed to connect to Wayland display." << std::endl;
        return 1;
    }

    auto window = display->create_window();

    core::event_loop::main().run();

    return 0;
}
