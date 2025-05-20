#pragma once

#include <functional>
#include <cairo/cairo.h>
#include <GLES3/gl3.h>

class Layer final {
public:
    Layer(float x, float y, float width, float height);
    ~Layer();

    auto draw() -> void;
    auto composite() -> void;

    std::function<void(cairo_t*)> on_draw {};

private:
    float m_x {};
    float m_y {};
    float m_width {};
    float m_height {};

    unsigned int m_vao;
    unsigned int m_vbo;

    GLuint m_texture {};
};
