#pragma once

#include "Display.h"
#include <wayland-cursor.h>

namespace GUI::Wayland
{
    class Cursor
    {
    public:
        Cursor(const RefPtr<Display>& display);
        ~Cursor();

        wl_surface* wlSurface() const;
        wl_cursor_image* wlCursorImage() const;

    private:
        wl_cursor_theme* _wlCursorTheme {};
        wl_cursor_image* _wlCursorImage {};
        wl_surface* _wlSurface {};
    };

    inline wl_surface* Cursor::wlSurface() const
    {
        return _wlSurface;
    }

    inline wl_cursor_image* Cursor::wlCursorImage() const
    {
        return _wlCursorImage;
    }
}
