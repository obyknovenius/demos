#include "window_decoration_view.h"

#include "window_title_bar.h"
#include <gfx/color.h>
#include <gfx/context.h>

namespace gui
{
    window::decoration_view::decoration_view(const gfx::rect& frame) : view { frame }
    {
        auto title_bar = make_ref_counted<window::title_bar>(gfx::rect(0, 0, frame.width, 30));
        add_subview(title_bar);
    }

    void window::decoration_view::redraw(nonnull_ref_ptr<gfx::context> context)
    {
        view::redraw(context);

        int line_width = 2;
        context->stroke_rect(m_frame.inset(line_width / 2), gfx::color::black, line_width);
    }
}
