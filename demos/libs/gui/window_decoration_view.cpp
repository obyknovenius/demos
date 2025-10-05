#include "window_decoration_view.h"

#include <gfx/color.h>
#include <gfx/context.h>

namespace gui
{
    void window::decoration_view::redraw(nonnull_ref_ptr<gfx::context> context)
    {
        int line_width = 2;
        context->stroke_rect(m_frame.inset(line_width / 2), gfx::color::black, line_width);

        int y = 5;
        for (int i = 0; i < 6; ++i, y += 4)
            context->stroke_line({4, y}, {m_frame.width - 4, y}, gfx::color::black, 2.0f);
        context->stroke_line({0, y}, {m_frame.width, y}, gfx::color::black, 2.0f);
    }
}
