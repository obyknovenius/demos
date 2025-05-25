#include "text_view.h"

#include <string>

namespace ui {

auto TextView::intrinsic_size() -> std::optional<gfx::Size>
{
    gfx::Size intrinsic_size {};

    cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 0, 0);
    cairo_t* cr = cairo_create(surface);

    cairo_select_font_face(cr, m_font_face.c_str(), CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, m_font_size);

    cairo_font_extents_t font_extents;
    cairo_font_extents(cr, &font_extents);
    intrinsic_size.height = font_extents.ascent + font_extents.descent;

    cairo_text_extents_t text_extents;
    cairo_text_extents(cr, m_text.c_str(), &text_extents);
    intrinsic_size.width = text_extents.width;

    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    return intrinsic_size;
}

auto TextView::draw(cairo_t* cr) -> void
{
    cairo_rectangle(cr, 0.0, 0.0, m_frame.width, m_frame.height);
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_fill(cr);


    cairo_select_font_face(cr, m_font_face.c_str(), CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, m_font_size);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);

    cairo_font_extents_t font_extents;
    cairo_font_extents(cr, &font_extents);

    cairo_text_extents_t text_extents;
    cairo_text_extents(cr, m_text.c_str(), &text_extents);

    cairo_move_to(cr, -text_extents.x_bearing, font_extents.ascent);
    cairo_show_text(cr, m_text.c_str());
    cairo_stroke(cr);
}

}
