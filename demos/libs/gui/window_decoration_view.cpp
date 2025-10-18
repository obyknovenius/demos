#include "window_decoration_view.h"

#include "window_title_bar.h"
#include <gfx/color.h>
#include <gfx/context.h>

namespace gui
{
    window::decoration_view::decoration_view() :
        view {},
        m_title_bar { make_ref_counted<window::title_bar>() }
    {
        add_subview(m_title_bar);
    }

    void window::decoration_view::layout()
    {
        m_title_bar->set_frame({ 6, 2, m_frame.width - 12, 30 });

        view::layout();
    }

    void window::decoration_view::redraw(nonnull_ref_ptr<gfx::context> context)
    {
        context->fill_rect(m_frame, m_background_color);

        int line_width = 2;
        context->stroke_rect(m_frame.inset(line_width / 2), gfx::color::black, line_width);

        context->stroke_rect(m_frame.inset(line_width + 1 + 4), gfx::color::black, line_width);

        for (auto& subview : m_subviews)
        {
            context->translate(subview->get_frame().x, subview->get_frame().y);
            subview->redraw(context);
        }
    }
}
