#pragma once

#include <functional>
#include <cairo/cairo.h>
#include <GLES3/gl3.h>

class Layer final {
public:
    Layer(int witdh, int height);
    ~Layer();

    auto texture() const -> GLuint { return m_texture; }

    auto draw() -> void;

    std::function<void(cairo_t*)> on_draw {};

private:
    int m_width {};
    int m_height {};

    GLuint m_texture {};
};
