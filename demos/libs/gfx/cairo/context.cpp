#include "context.h"

#include "../color.h"
#include "../rect.h"

namespace gfx::cairo {

auto context::stroke_rect(const rect& rect, const color& color, float line_width) -> void
{
    cairo_save(m_cr);
    cairo_set_line_width(m_cr, line_width);
    cairo_set_source_rgba(m_cr, color.r, color.g, color.b, color.a);
    cairo_rectangle(m_cr, rect.x, rect.y, rect.width, rect.height);
    cairo_stroke(m_cr);
    cairo_restore(m_cr);
}

auto context::fill_rect(const rect& rect, const color& color) -> void
{
    cairo_save(m_cr);
    cairo_set_source_rgba(m_cr, color.r, color.g, color.b, color.a);
    cairo_rectangle(m_cr, rect.x, rect.y, rect.width, rect.height);
    cairo_fill(m_cr);
    cairo_restore(m_cr);
}

}
