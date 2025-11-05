#include "Cursor.h"

namespace GUI::Wayland
{
    Cursor::Cursor(const RefPtr<Display>& display)
    {
        if (!display)
            return;

        _wlCursorTheme = wl_cursor_theme_load(nullptr, 32, display->wlShm());
        wl_cursor* wlCursor = wl_cursor_theme_get_cursor(_wlCursorTheme, "left_ptr");
        _wlCursorImage = wlCursor->images[0];
        wl_buffer* wlBuffer = wl_cursor_image_get_buffer(_wlCursorImage);

        _wlSurface = wl_compositor_create_surface(display->wlCompositor());
        wl_surface_attach(_wlSurface, wlBuffer, 0, 0);
        wl_surface_commit(_wlSurface);
    }

    Cursor::~Cursor()
    {
        if (_wlSurface)
            wl_surface_destroy(_wlSurface);
        if (_wlCursorTheme)
            wl_cursor_theme_destroy(_wlCursorTheme);
    }
}
