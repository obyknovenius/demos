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

        wl_surface* getWlSurface() const;
        wl_cursor_image* getWlCursorImage() const;

    private:
        wl_cursor_theme* _wlCursorTheme {};
        wl_cursor_image* _wlCursorImage {};
        wl_surface* _wlSurface {};
    };

    inline wl_surface* Cursor::getWlSurface() const
    {
        return _wlSurface;
    }

    inline wl_cursor_image* Cursor::getWlCursorImage() const
    {
        return _wlCursorImage;
    }
}
