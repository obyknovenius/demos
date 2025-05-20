#include "window_close_button.h"

auto WindowCloseButton::draw(cairo_t* cr) -> void
{
    WindowButton::draw(cr);

    auto cross_padding = 5.0f;
    cairo_move_to(cr, m_margin + cross_padding, cross_padding);
    cairo_line_to(cr, m_width - m_margin - cross_padding, m_height - cross_padding);
    cairo_move_to(cr, m_width - m_margin - cross_padding, cross_padding);
    cairo_line_to(cr, m_margin + cross_padding, m_height - cross_padding);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_set_line_width(cr, 2.0f);
    cairo_stroke(cr);
}
