#include "window_title_bar.h"

#include <gfx/color.h>
#include <gfx/context.h>

namespace gui
{
    void window::title_bar::redraw(nonnull_ref_ptr<gfx::context> context)
    {
        context->fill_rect(m_bounds, gfx::color::white);
        int y = 5;
        for (int i = 0; i < 6; ++i, y += 4)
            context->stroke_line({0, y}, {m_bounds.width, y}, gfx::color::black, 2.0f);
        y += 1;
        context->stroke_line({0, y}, {m_bounds.width, y}, gfx::color::black, 2.0f);
    }
}
