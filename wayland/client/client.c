
#include "display.h"
#include "window.h"
#include <stdlib.h>
#include <string.h>
#include <wayland-client.h>

int
main(int argc, char *argv[])
{
    struct display *display;
    struct window *window;
    int width = 640, height = 480;

    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "-w") == 0) {
            width = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-h") == 0) {
            height = atoi(argv[i + 1]);
        }
    }

    display = create_display();
    window = create_window(display, width, height);

    while (wl_display_dispatch(display->wl_display)) {
        /* This space deliberately left blank */
    }

    destroy_window(window);
    destroy_display(display);

    return 0;
}
