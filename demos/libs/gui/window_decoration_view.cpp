#include "window_decoration_view.h"

#include "window_title_bar.h"
#include <Gfx/Color.h>
#include <Gfx/Context.h>

namespace gui
{
    window::decoration_view::decoration_view(NonnullRefPtr<window> window) :
        view {},
        m_window { window },
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

    void window::decoration_view::redraw(NonnullRefPtr<Gfx::Context> context)
    {
        context->fillRect(m_frame, Gfx::Color::white);

        int line_width = 2;

        auto inner_border_rect = m_frame.inset(m_border_thickness);
        context->strokeRect(inner_border_rect.inset(line_width / 2), Gfx::Color::black, line_width);

        view::redraw(context);

        auto outer_border_rect = m_frame;
        context->strokeRect(outer_border_rect.inset(line_width / 2), Gfx::Color::black, line_width);
    }

    void window::decoration_view::on_button_pressed(std::unique_ptr<const event> event)
    {
        if (auto window = m_window.strong())
        {
            window->close();
        }
    }
}
