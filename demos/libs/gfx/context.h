#pragma once

#include <core/ref_counted.h>

namespace gfx
{
    struct color;
    struct rect;
    struct point;

    class context : public ref_counted
    {
    public:
        virtual auto stroke_line(const point& from, const point& to, const color& color, float line_width = 1.0f) -> void = 0;
        virtual auto stroke_rect(const rect& rect, const color& color, float line_width = 1.0f) -> void = 0;
        virtual auto fill_rect(const rect& rect, const color& color) -> void = 0;

    protected:
        context() = default;
        virtual ~context() = default;
    };
}
