#include "Context.h"

#include "../Color.h"
#include "../Point.h"
#include "../Rect.h"

namespace Gfx::Cairo
{
    void Context::translate(const Gfx::Point& offset)
    {
        cairo_translate(_cr, offset.x, offset.y);
    }

    void Context::clip(const Gfx::Rect& rect)
    {
        cairo_rectangle(_cr, rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
        cairo_clip(_cr);
    }

    void Context::strokeLine(const Gfx::Point& from, const Gfx::Point& to, const Gfx::Color& color, float lineWidth)
    {
        cairo_save(_cr);
        cairo_set_line_width(_cr, lineWidth);
        cairo_set_source_rgba(_cr, color.r, color.g, color.b, color.a);
        cairo_move_to(_cr, from.x, from.y);
        cairo_line_to(_cr, to.x, to.y);
        cairo_stroke(_cr);
        cairo_restore(_cr);
    }

    void Context::strokeRect(const Gfx::Rect& rect, const Gfx::Color& color, float lineWidth)
    {
        cairo_save(_cr);
        cairo_set_line_width(_cr, lineWidth);
        cairo_set_source_rgba(_cr, color.r, color.g, color.b, color.a);
        cairo_rectangle(_cr, rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
        cairo_stroke(_cr);
        cairo_restore(_cr);
    }

    void Context::fillRect(const Gfx::Rect& rect, const Gfx::Color& color)
    {
        cairo_save(_cr);
        cairo_set_source_rgba(_cr, color.r, color.g, color.b, color.a);
        cairo_rectangle(_cr, rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
        cairo_fill(_cr);
        cairo_restore(_cr);
    }
}
