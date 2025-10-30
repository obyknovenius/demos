#pragma once

#include "../context.h"
#include <cairo/cairo.h>
#include <Core/NonnullRefPtr.h>

namespace gfx::cairo
{
    class context final : public gfx::context
    {
    public:
        static NonnullRefPtr<context> make(cairo_t* cr);

        inline void save() override;
        inline void restore() override;

        void translate(const gfx::point& offset) override;

        void clip(const gfx::rect& rect) override;

        void stroke_line(const point& from, const point& to, const color& color, float line_width) override;
        void stroke_rect(const rect& rect, const color& color, float line_width) override;
        void fill_rect(const rect& rect, const color& color) override;

    private:
        context(cairo_t* cr) : m_cr { cr } {}
        ~context();

        cairo_t* m_cr;
    };

    inline NonnullRefPtr<context> context::make(cairo_t* cr)
    {
        return adopt(*new context(cr));
    }

    inline context::context(cairo_t* cr) : m_cr { cr }
    {
    }

    inline context::~context()
    {
        cairo_destroy(m_cr);
    }

    void context::save()
    {
        cairo_save(m_cr);
    }

    void context::restore()
    {
        cairo_restore(m_cr);
    }
}
