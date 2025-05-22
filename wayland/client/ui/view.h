#pragma once

#include <cairo/cairo.h>
#include <optional>

#include <gfx/rect.h>
#include <gfx/size.h>

class View {
public:
    View() = default;

    explicit View(const gfx::Rect& frame) : m_frame { frame }
    {
    }

    virtual ~View() = default;

    auto set_frame(const gfx::Rect& frame) -> void { m_frame = frame; }
    auto frame() const -> gfx::Rect { return m_frame; }

    virtual auto intrinsic_size() -> std::optional<gfx::Size> { return std::nullopt; }

    virtual auto draw(cairo_t*) -> void {}

protected:
    gfx::Rect m_frame {};
};
