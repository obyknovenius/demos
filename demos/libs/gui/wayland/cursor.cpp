#include "cursor.h"

namespace gui::wayland
{
    cursor::cursor(const ref_ptr<display>& display)
    {
        if (!display)
            return;

        m_wl_cursor_theme = wl_cursor_theme_load(nullptr, 32, display->get_wl_shm());
        wl_cursor* wl_cursor = wl_cursor_theme_get_cursor(m_wl_cursor_theme, "left_ptr");
        m_wl_cursor_image = wl_cursor->images[0];
        wl_buffer* wl_buffer = wl_cursor_image_get_buffer(m_wl_cursor_image);

        m_wl_surface = wl_compositor_create_surface(display->get_wl_compositor());
        wl_surface_attach(m_wl_surface, wl_buffer, 0, 0);
        wl_surface_commit(m_wl_surface);
    }

    cursor::~cursor()
    {
        if (m_wl_surface)
            wl_surface_destroy(m_wl_surface);
        if (m_wl_cursor_theme)
            wl_cursor_theme_destroy(m_wl_cursor_theme);
    }
}
