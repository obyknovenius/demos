#include "view.h"

namespace gui
{
    void view::set_frame(const gfx::rect& frame)
    {
        m_frame = frame;
        m_bounds = { { 0, 0 }, frame.size };
    }

    void view::layout()
    {
        for (const auto& subview : m_subviews)
            subview->layout();
    }

    void view::redraw(nonnull_ref_ptr<gfx::context> context)
    {
        for (const auto& subview : m_subviews)
        {
            context->save();
            context->clip(subview->get_frame());
            context->translate(subview->get_frame().origin);
            subview->redraw(context);
            context->restore();
        }
    }

    ref_ptr<view> view::hit_test(const gfx::point& point)
    {
        for (const auto& subview : m_subviews)
            if (subview->get_frame().contains(point))
                return subview->hit_test(point - subview->get_frame().origin);
        return this;
    }
}
