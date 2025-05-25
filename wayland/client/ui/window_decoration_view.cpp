#include "window_decoration_view.h"

#include <string>

#include "title_bar.h"

namespace ui {

Window::DecorationView::DecorationView(const std::string& text, const gfx::Rect& frame)
    : View { frame }
{
    m_title_bar = std::make_shared<TitleBar>(text);
    add_subview(std::static_pointer_cast<View>(m_title_bar));
}

auto Window::DecorationView::layout() -> void
{
    m_title_bar->set_frame({ m_frame.x, m_frame.y, m_frame.width, 36.0f });

    View::layout();
}

auto Window::DecorationView::draw(cairo_t* cr) -> void
{
    cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1.0);
    cairo_paint(cr);

    cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 1.0);

    cairo_rectangle(cr, 1.0, 1.0, m_frame.width - 2.0, m_frame.height - 2.0);
    cairo_stroke(cr);

    View::draw(cr);
}

}
