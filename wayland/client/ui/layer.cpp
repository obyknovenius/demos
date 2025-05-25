#include "layer.h"

#include <cmath>

#ifndef GL_BGRA
#define GL_BGRA 0x80E1
#endif

namespace ui {

Layer::Layer(const gfx::Rect& frame)
    : m_frame { frame }
{
    float vertices[] = {
        // positions                         // texture coordinates
        frame.min_x(), frame.max_y(), 0.0f,  0.0f, 0.0f,  // bottom left
        frame.min_x(), frame.min_y(), 0.0f,  0.0f, 1.0f,  // top left
        frame.max_x(), frame.max_y(), 0.0f,  1.0f, 0.0f,  // bottom right
        frame.max_x(), frame.min_y(), 0.0f,  1.0f, 1.0f   // top right
    };

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &m_vbo);

    glDeleteTextures(1, &m_texture);
}

auto Layer::draw() -> void
{
    if (on_draw) {
        auto surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, std::floor(m_frame.width), std::floor(m_frame.height));
        auto cr = cairo_create(surface);

        cairo_translate(cr, 0, m_frame.height);
        cairo_scale(cr, 1, -1);
        
        on_draw(cr);
        cairo_surface_flush(surface);

        GLint bound_texture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound_texture);

        glBindTexture(GL_TEXTURE_2D, m_texture);
        auto* data = cairo_image_surface_get_data(surface);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_frame.width, m_frame.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);

        glBindTexture(GL_TEXTURE_2D, bound_texture);

        cairo_destroy(cr);
        cairo_surface_destroy(surface);
    }
}

auto Layer::composite() -> void
{
    glBindVertexArray(m_vao);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

}
