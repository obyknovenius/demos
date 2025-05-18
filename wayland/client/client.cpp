
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <wayland-client.h>
#include "display.h"
#include "window.h"

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

    Display display {};
    Window window { display, width, height };

    while (true) {
        if (wl_display_dispatch(display.wl_display()) < 0) {
            if (errno != EAGAIN) {
                perror("wl_display_dispatch");
                break;
            }
        }
    }

    return 0;
}
