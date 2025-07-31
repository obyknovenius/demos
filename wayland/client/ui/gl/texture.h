#pragma once

#include <GLES3/gl3.h>
#include <gfx/rect.h>

namespace gl {

class Texture final {
public:
    Texture(const gfx::Rect& rect, const void* pixels);
    ~Texture();

    auto draw() const -> void;

private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_texture;
};

}
