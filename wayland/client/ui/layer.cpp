#include "layer.h"

#include <cmath>

#ifndef GL_BGRA
#define GL_BGRA 0x80E1
#endif

namespace ui {

Layer::Layer(const gfx::Rect& frame)
    : m_frame { frame }
    , m_surface { cairo_image_surface_create(CAIRO_FORMAT_ARGB32, std::floor(frame.width), std::floor(frame.height)) }
{
}

Layer::~Layer()
{
    cairo_surface_destroy(m_surface);
}

auto Layer::draw() -> void
{
    if (on_draw) {
        auto cr = cairo_create(m_surface);

        cairo_translate(cr, 0, m_frame.height);
        cairo_scale(cr, 1, -1);
        
        on_draw(cr);
        cairo_surface_flush(m_surface);

        cairo_destroy(cr);
    }
}

}
