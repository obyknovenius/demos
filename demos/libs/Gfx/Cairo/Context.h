#pragma once

#include "../Context.h"
#include <cairo/cairo.h>
#include <Core/RefPtr.h>

namespace Gfx::Cairo
{
    class Context final : public Gfx::Context
    {
    public:
        static RefPtr<Context> make(cairo_t* cr);

        inline void save() override;
        inline void restore() override;

        void translate(const Gfx::Point& offset) override;

        void clip(const Gfx::Rect& rect) override;

        void strokeLine(const Gfx::Point& from, const Gfx::Point& to, const Gfx::Color& color, float lineWidth) override;
        void strokeRect(const Gfx::Rect& rect, const Gfx::Color& color, float lineWidth) override;
        void fillRect(const Gfx::Rect& rect, const Gfx::Color& color) override;

    private:
        Context(cairo_t* cr);
        ~Context();

        cairo_t* _cr;
    };

    inline RefPtr<Context> Context::make(cairo_t* cr)
    {
        return adopt(new Context(cr));
    }

    inline Context::Context(cairo_t* cr) : _cr { cr }
    {
    }

    inline Context::~Context()
    {
        cairo_destroy(_cr);
    }

    void Context::save()
    {
        cairo_save(_cr);
    }

    void Context::restore()
    {
        cairo_restore(_cr);
    }
}
