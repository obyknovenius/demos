#pragma once

#include <core/ref_counted.h>
#include <gfx/color.h>
#include <gfx/context.h>
#include <gfx/rect.h>
#include <vector>

namespace gui
{
    class view : public ref_counted
    {
    public:
        void set_frame(const gfx::rect& frame);
        gfx::rect get_frame() const { return m_frame; }

        virtual gfx::size intrinsic_size() { return { -1, -1 }; }

        inline void add_subview(nonnull_ref_ptr<view> subview);

        virtual void layout();
        virtual void redraw(nonnull_ref_ptr<gfx::context> context);

    protected:
        view() = default;
        virtual ~view() = default;

    protected:
        gfx::rect m_frame {};
        gfx::rect m_bounds {};

        std::vector<nonnull_ref_ptr<view>> m_subviews {};
    };

    void view::add_subview(nonnull_ref_ptr<view> subview)
    {
        m_subviews.push_back(subview);
    }
}
