#pragma once

#include <core/ref_ptr.h>
#include <core/weak_ptr.h>
#include <core/weakable.h>
#include <gfx/color.h>
#include <gfx/context.h>
#include <gfx/rect.h>
#include <memory>
#include <vector>

namespace gui
{
    class event;

    class view : public weakable
    {
    public:
        ref_ptr<view> get_superview();
        void add_subview(nonnull_ref_ptr<view> subview);

        void set_frame(const gfx::rect& frame);
        gfx::rect get_frame() const { return m_frame; }

        virtual gfx::size intrinsic_size() { return { -1, -1 }; }

        virtual void layout();
        virtual void redraw(nonnull_ref_ptr<gfx::context> context);

        ref_ptr<view> hit_test(const gfx::point& point);

        virtual void on_button_pressed(std::unique_ptr<const event> event) {};

    protected:
        view() = default;
        virtual ~view() = default;

    protected:
        weak_ptr<view> m_superview {};
        std::vector<nonnull_ref_ptr<view>> m_subviews {};

        gfx::rect m_frame {};
        gfx::rect m_bounds {};
    };

    inline ref_ptr<view> view::get_superview()
    {
        return m_superview.strong_ref();
    }

    inline void view::add_subview(nonnull_ref_ptr<view> subview)
    {
        subview->m_superview = this;
        m_subviews.push_back(subview);
    }
}
