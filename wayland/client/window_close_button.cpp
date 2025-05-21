#include "window_close_button.h"

auto WindowCloseButton::draw(cairo_t* cr) -> void
{
    WindowButton::draw(cr);

    auto bounds = gfx::Rect { 0.0f, 0.0f, m_frame.width, m_frame.height };
    auto cross_padding = 5.0f;
    auto cross_rect = bounds.inset_by(cross_padding, cross_padding);
    
    cairo_move_to(cr, cross_rect.min_x(), cross_rect.min_y());
    cairo_line_to(cr, cross_rect.max_x(), cross_rect.max_y());
    cairo_move_to(cr, cross_rect.max_x(), cross_rect.min_y());
    cairo_line_to(cr, cross_rect.min_x(), cross_rect.max_y());
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_set_line_width(cr, 2.0f);
    cairo_stroke(cr);
}
