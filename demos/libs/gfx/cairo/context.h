#pragma once

#include "../context.h"
#include <cairo/cairo.h>

namespace gfx::cairo {

class context final : public gfx::context
{
    template<typename T, class... Args>
    friend nonnull_ref_ptr<T> core::make_ref_counted(Args&&...);

public:
    auto stroke_rect(const rect& rect, const color& color, float line_width) -> void override;
    auto fill_rect(const rect& rect, const color& color) -> void override;

private:
    context(cairo_t* cr) : m_cr { cr } {}
    ~context() { cairo_destroy(m_cr); }

    cairo_t* m_cr {};
};

}
