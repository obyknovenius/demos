#pragma once

#include <functional>
#include <cairo/cairo.h>
#include <gfx/rect.h>

namespace ui {

class Layer final {
public:
    Layer(const gfx::Rect& frame);
    ~Layer();

    auto frame() const -> const gfx::Rect& { return m_frame; }
    auto data() const -> void* { return cairo_image_surface_get_data(m_surface); }

    auto draw() -> void;

    std::function<void(cairo_t*)> on_draw {};

private:
    gfx::Rect m_frame {};

    cairo_surface_t* m_surface {};
};

}
