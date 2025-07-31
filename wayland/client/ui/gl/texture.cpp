#include "texture.h"

#ifndef GL_BGRA
#define GL_BGRA 0x80E1
#endif

namespace gl {

Texture::Texture(const gfx::Rect& rect, const void* pixels)
{
    float vertices[] = {
        // positions                       // texture coordinates
        rect.min_x(), rect.max_y(), 0.0f,  0.0f, 0.0f,  // bottom left
        rect.min_x(), rect.min_y(), 0.0f,  0.0f, 1.0f,  // top left
        rect.max_x(), rect.max_y(), 0.0f,  1.0f, 0.0f,  // bottom right
        rect.max_x(), rect.min_y(), 0.0f,  1.0f, 1.0f   // top right
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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rect.width, rect.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, bound_texture);
}

Texture::~Texture()
{
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &m_vbo);

    glDeleteTextures(1, &m_texture);
}

auto Texture::draw() const -> void
{
    glBindVertexArray(m_vao);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

}
