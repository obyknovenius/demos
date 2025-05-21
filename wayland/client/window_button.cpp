#include "window_close_button.h"

auto WindowButton::draw(cairo_t* cr) -> void
{
    auto bounds = gfx::Rect { 0.0f, 0.0f, m_frame.width, m_frame.height };

    cairo_rectangle(cr, bounds.x, bounds.y, bounds.width, bounds.height);
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_fill(cr);

    auto line_width = 2.0f;
    auto border_rect = bounds.inset_by(line_width / 2, line_width / 2);
    cairo_rectangle(cr, border_rect.x, border_rect.y, border_rect.width, border_rect.height);
    cairo_set_line_width(cr, line_width);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_stroke(cr);
}
