#pragma once

#include "../context.h"
#include <cairo/cairo.h>

namespace gfx::cairo {

class context final : public gfx::context
{
public:
    static auto create(cairo_t* cr) -> nonnull_ref_ptr<context>
    {
        return adopt(*new context(cr));
    }

    auto stroke_line(const point& from, const point& to, const color& color, float line_width) -> void override;
    auto stroke_rect(const rect& rect, const color& color, float line_width) -> void override;
    auto fill_rect(const rect& rect, const color& color) -> void override;

private:
    context(cairo_t* cr) : m_cr { cr } {}
    ~context() { cairo_destroy(m_cr); }

    cairo_t* m_cr {};
};

}
