#pragma once

#include <functional>
#include <cairo/cairo.h>
#include <GLES3/gl3.h>
#include <gfx/rect.h>

namespace ui {

class Layer final {
public:
    Layer(const gfx::Rect& frame);
    ~Layer();

    auto draw() -> void;
    auto composite() -> void;

    std::function<void(cairo_t*)> on_draw {};

private:
    gfx::Rect m_frame;

    unsigned int m_vao;
    unsigned int m_vbo;

    GLuint m_texture {};
};

}
