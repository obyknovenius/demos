#include "context.h"

#include "../color.h"
#include "../point.h"
#include "../rect.h"

namespace gfx::cairo
{
    void context::translate(const gfx::point& offset)
    {
        cairo_translate(m_cr, offset.x, offset.y);
    }

    void context::clip(const gfx::rect& rect)
    {
        cairo_rectangle(m_cr, rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
        cairo_clip(m_cr);
    }

    void context::stroke_line(const point& from, const point& to, const color& color, float line_width)
    {
        cairo_save(m_cr);
        cairo_set_line_width(m_cr, line_width);
        cairo_set_source_rgba(m_cr, color.r, color.g, color.b, color.a);
        cairo_move_to(m_cr, from.x, from.y);
        cairo_line_to(m_cr, to.x, to.y);
        cairo_stroke(m_cr);
        cairo_restore(m_cr);
    }

    void context::stroke_rect(const rect& rect, const color& color, float line_width)
    {
        cairo_save(m_cr);
        cairo_set_line_width(m_cr, line_width);
        cairo_set_source_rgba(m_cr, color.r, color.g, color.b, color.a);
        cairo_rectangle(m_cr, rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
        cairo_stroke(m_cr);
        cairo_restore(m_cr);
    }

    void context::fill_rect(const rect& rect, const color& color)
    {
        cairo_save(m_cr);
        cairo_set_source_rgba(m_cr, color.r, color.g, color.b, color.a);
        cairo_rectangle(m_cr, rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
        cairo_fill(m_cr);
        cairo_restore(m_cr);
    }
}
