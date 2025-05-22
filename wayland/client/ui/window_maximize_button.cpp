#include "window_maximize_button.h"

auto WindowMaximizeButton::draw(cairo_t* cr) -> void
{
    WindowButton::draw(cr);
 
    auto line_width = 2.0f;
    cairo_rectangle(cr,
                    line_width / 2,
                    line_width / 2,
                    14.0f - line_width,
                    14.0f - line_width);
    cairo_set_line_width(cr, line_width);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_stroke(cr);
}
