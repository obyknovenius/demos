#include "view.h"

namespace gui
{
    void view::redraw(nonnull_ref_ptr<gfx::context> context)
    {
        for (auto& subview : m_subviews)
            subview->redraw(context);
    }
}
