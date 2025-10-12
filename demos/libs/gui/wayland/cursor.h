#pragma once

#include "display.h"
#include <wayland-cursor.h>

namespace gui::wayland
{
    class cursor
    {
    public:
        cursor(const ref_ptr<display>& display);
        ~cursor();

        wl_surface* get_wl_surface() const { return m_wl_surface; }
        wl_cursor_image* get_wl_cursor_image() const { return m_wl_cursor_image; }
    
    private:
        wl_cursor_theme* m_wl_cursor_theme {};
        wl_cursor_image* m_wl_cursor_image {};
        wl_surface* m_wl_surface {};
    };
}
