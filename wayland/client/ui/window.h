#pragma once

#include <cairo/cairo.h>
#include <gfx/size.h>
#include <gfx/rect.h>
#include <memory>

namespace ui {

class Display;
class Layer;

class Window final {
public:
    class DecorationView;

    static auto create(const gfx::Size& frame) -> std::shared_ptr<Window>;

    static auto with_id(int id) -> std::shared_ptr<Window>;

    Window(const gfx::Size& size);
    ~Window();

    auto layer() const -> Layer* { return m_layer; }

    auto draw() -> void;

private:
    auto draw(cairo_t* cr) -> void;
    auto draw_titlebar(cairo_t* cr) -> void;
    auto draw_placeholder(cairo_t* cr, const gfx::Rect& rect, float dx) -> void;

    int m_id;

    gfx::Size m_size {};

    Layer* m_layer {};

    std::shared_ptr<DecorationView> m_decoration_view {};
};

}
