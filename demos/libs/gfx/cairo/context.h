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

        inline void save() override;
        inline void restore() override;

        void translate(const gfx::point& offset) override;

        void clip(const gfx::rect& rect) override;

        void stroke_line(const point& from, const point& to, const color& color, float line_width) override;
        void stroke_rect(const rect& rect, const color& color, float line_width) override;
        void fill_rect(const rect& rect, const color& color) override;

    private:
        cairo_t* m_cr;
    };

    void context::save()
    {
        cairo_save(m_cr);
    }

    void context::restore()
    {
        cairo_restore(m_cr);
    }
}
