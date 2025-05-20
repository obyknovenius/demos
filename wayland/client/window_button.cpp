#include "window_close_button.h"

auto WindowButton::draw(cairo_t* cr) -> void
{
    cairo_rectangle(cr, 0.0, 0.0, m_width, m_height);
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_fill(cr);

    cairo_save(cr);
    cairo_translate(cr, m_margin, 0.0f);

    auto line_width = 2.0f;
    cairo_rectangle(cr,
                    line_width / 2,
                    line_width / 2,
                    m_size - line_width,
                    m_size - line_width);
    cairo_set_line_width(cr, line_width);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_stroke(cr);

    cairo_restore(cr);
}
