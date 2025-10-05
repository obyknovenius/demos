#pragma once

#include <core/ref_counted.h>
#include <gfx/context.h>
#include <gfx/rect.h>

namespace gui
{
    class view : public ref_counted
    {
    public:
        virtual void redraw(nonnull_ref_ptr<gfx::context> context) = 0;

    protected:
        view(const gfx::rect& frame) : m_frame { frame } {}
        virtual ~view() = default;

    protected:
        gfx::rect m_frame {};
    };
}
