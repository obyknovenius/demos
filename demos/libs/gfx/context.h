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
        virtual void save() = 0;
        virtual void restore() = 0;

        virtual void translate(const point& offset) = 0;

        virtual void clip(const rect& rect) = 0;

        virtual void stroke_line(const point& from, const point& to, const color& color, float line_width = 1.0f) = 0;
        virtual void stroke_rect(const rect& rect, const color& color, float line_width = 1.0f) = 0;
        virtual void fill_rect(const rect& rect, const color& color) = 0;

    protected:
        context() = default;
        virtual ~context() = default;
    };
}
