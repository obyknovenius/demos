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
        for (auto& subview : m_subviews)
            subview->layout();
    }

    void view::redraw(nonnull_ref_ptr<gfx::context> context)
    {
        for (auto& subview : m_subviews)
        {
            context->save();
            context->clip(subview->get_frame());
            context->translate(subview->get_frame().origin);
            subview->redraw(context);
            context->restore();
        }
    }
}
