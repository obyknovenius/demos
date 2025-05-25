#pragma once

#include <cairo/cairo.h>
#include <gfx/rect.h>
#include <gfx/size.h>
#include <optional>
#include <memory>
#include <vector>

namespace ui {

class View {
public:
    explicit View(const gfx::Rect& frame = {}) : m_frame { frame } {}
    virtual ~View() = default;

    auto add_subview(std::shared_ptr<View> subview) -> void { m_subviews.push_back(subview); }

    auto set_frame(const gfx::Rect& frame) -> void { m_frame = frame; }
    auto frame() const -> gfx::Rect { return m_frame; }

    virtual auto intrinsic_size() -> std::optional<gfx::Size> { return std::nullopt; }

    virtual auto layout() -> void;

    virtual auto draw(cairo_t* cr) -> void;

protected:
    gfx::Rect m_frame {};

    std::vector<std::shared_ptr<View>> m_subviews {};
};

}
