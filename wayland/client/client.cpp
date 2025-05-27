
#include <errno.h>
#include <gfx/rect.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ui/display_server.h>
#include <ui/window.h>

int main(int argc, char* argv[])
{
    int width = 640, height = 480;

    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "-w") == 0) {
            width = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-h") == 0) {
            height = atoi(argv[i + 1]);
        }
    }

    auto display_server = ui::DisplayServer::shared();

    auto window { ui::Window::create(gfx::Size { static_cast<float>(width), static_cast<float>(height) }) };

    display_server->run();

    return 0;
}
