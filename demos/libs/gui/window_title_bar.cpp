#include "window_title_bar.h"

#include <gfx/color.h>
#include <gfx/context.h>

namespace gui
{
    void window::title_bar::redraw(NonnullRefPtr<gfx::context> context)
    {
        context->fill_rect(m_bounds, gfx::color::white);

        int padding = 4;
        int y = 6;
        for (int i = 0; i < 6; ++i, y += 4)
            context->stroke_line({ padding, y }, { m_bounds.size.width - padding, y }, gfx::color::black, 2.0f);
        y += 1;
        context->stroke_line({ padding, y }, { m_bounds.size.width - padding, y }, gfx::color::black, 2.0f);
    }
}
