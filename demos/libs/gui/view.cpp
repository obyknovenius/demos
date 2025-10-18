#include "view.h"

namespace gui
{
    void view::set_frame(const gfx::rect& frame)
    {
        m_frame = frame;
        m_bounds = { 0, 0, frame.width, frame.height };
    }

    void view::layout()
    {
        for (auto& subview : m_subviews)
            subview->layout();
    }

    void view::redraw(nonnull_ref_ptr<gfx::context> context)
    {
        for (auto& subview : m_subviews)
            subview->redraw(context);
    }
}
