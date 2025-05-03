#include "layer.h"

#ifndef GL_BGRA
#define GL_BGRA 0x80E1
#endif

Layer::Layer(int width, int height)
{
    m_width = width;
    m_height = height;

    GLint bound_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound_texture);

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, bound_texture);
}

Layer::~Layer()
{
    glDeleteTextures(1, &m_texture);
}

auto Layer::draw() -> void
{
    if (on_draw) {
        auto surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, m_width, m_height);
        auto cr = cairo_create(surface);
        
        on_draw(cr);
        cairo_surface_flush(surface);

        GLint bound_texture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound_texture);

        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE,
                    cairo_image_surface_get_data(surface));

        glBindTexture(GL_TEXTURE_2D, bound_texture);

        cairo_destroy(cr);
        cairo_surface_destroy(surface);
    }
}
