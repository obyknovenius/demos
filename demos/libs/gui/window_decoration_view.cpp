#include "window_decoration_view.h"

#include "window_title_bar.h"
#include <gfx/color.h>
#include <gfx/context.h>

namespace gui
{
    window::decoration_view::decoration_view() :
        view {},
        m_title_bar { title_bar::make() }
    {
        add_subview(m_title_bar);
    }

    void window::decoration_view::layout()
    {
        m_title_bar->set_frame({
            { 0, 0 },
            { m_frame.size.width, m_title_bar->intrinsic_size().height }
        });

        view::layout();
    }

    void window::decoration_view::redraw(nonnull_ref_ptr<gfx::context> context)
    {
        context->fill_rect(m_frame, gfx::color::white);

        int line_width = 2;

        auto inner_border_rect = m_frame.inset(m_border_thickness);
        context->stroke_rect(inner_border_rect.inset(line_width / 2), gfx::color::black, line_width);

        view::redraw(context);

        auto outer_border_rect = m_frame;
        context->stroke_rect(outer_border_rect.inset(line_width / 2), gfx::color::black, line_width);
    }
}
