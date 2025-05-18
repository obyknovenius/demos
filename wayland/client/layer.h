#pragma once

#include <functional>
#include <cairo/cairo.h>
#include <GLES3/gl3.h>

class Layer final {
public:
    Layer(float x, float y, float witdh, float height);
    ~Layer();

    auto draw() -> void;
    auto composite() -> void;

    std::function<void(cairo_t*)> on_draw {};

private:
    int m_width {};
    int m_height {};

    unsigned int m_vao;
    unsigned int m_vbo;

    GLuint m_texture {};
};
