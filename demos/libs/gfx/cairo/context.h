#pragma once

#include "../context.h"
#include <cairo/cairo.h>

namespace gfx::cairo
{
    class context final : public gfx::context
    {
    public:
        context(cairo_t* cr) : m_cr { cr }
        {
        }

        ~context()
        {
            cairo_destroy(m_cr);
        }

        void stroke_line(const point& from, const point& to, const color& color, float line_width) override;
        void stroke_rect(const rect& rect, const color& color, float line_width) override;
        void fill_rect(const rect& rect, const color& color) override;

    private:
        cairo_t* m_cr;
    };
}
