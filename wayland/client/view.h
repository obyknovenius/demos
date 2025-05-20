#pragma once

#include <cairo/cairo.h>

class View {
public:
    View(float x, float y, float width, float height)
        : m_x { x }
        , m_y { y }
        , m_width { width }
        , m_height { height }
    {}
    virtual ~View() = default;

    auto x() const -> float { return m_x; }
    auto set_x(float x) -> void { m_x = x; }

    auto y() const -> float { return m_y; }
    auto set_y(float y) -> void { m_y = y; }

    auto width() const -> float { return m_width; }
    auto set_width(float width) -> void { m_width = width; }

    auto height() const -> float { return m_height; }
    auto set_height(float height) -> void { m_height = height; }

    virtual auto draw(cairo_t*) -> void {}

protected:
    float m_x {};
    float m_y {};
    float m_width {};
    float m_height {};
};
