#include "title_bar.h"

#include "text_view.h"
#include "window_close_button.h"
#include "window_maximize_button.h"

#include <cassert>

TitleBar::TitleBar(const std::string& text, const gfx::Rect& frame)
    : View { frame }
{
    m_title_view = std::make_shared<TextView>(text);
    add_subview(std::static_pointer_cast<View>(m_title_view));

    m_close_button = std::make_shared<WindowCloseButton>();
    add_subview(std::static_pointer_cast<View>(m_close_button));

    m_maximize_button = std::make_shared<WindowMaximizeButton>();
    add_subview(std::static_pointer_cast<View>(m_maximize_button));
}

auto TitleBar::layout() -> void
{
    auto title_view_intrinsic_size = m_title_view->intrinsic_size();
    assert(title_view_intrinsic_size);

    m_close_button->set_frame({ 16.0f, 8.0f, 22.0f, 22.0f });

    m_title_view->set_frame({
        m_frame.width / 2 - title_view_intrinsic_size->width / 2,
        4.0f,
        title_view_intrinsic_size->width,
        title_view_intrinsic_size->height
    });

    m_maximize_button->set_frame({ m_frame.width - 22.0f - 16.0f, 8.0f, 22.0f, 22.0f });
}

auto TitleBar::draw(cairo_t* cr) -> void
{
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);

    cairo_save(cr);
    for (int i = 0; i < 6; ++i) {
        cairo_move_to(cr, 4.0, 9.0);
        cairo_line_to(cr, m_frame.width - 4.0, 9.0);
        cairo_stroke(cr);
        cairo_translate(cr, 0.0, 4.0);
    }
    cairo_restore(cr);

    cairo_move_to(cr, 0, m_frame.height - 1);
    cairo_line_to(cr, m_frame.width, m_frame.height - 1);
    cairo_stroke(cr);

    draw_placeholder(cr, m_close_button->frame(), -2.0f);
    draw_placeholder(cr, m_title_view->frame(), -16.0f);
    draw_placeholder(cr, m_maximize_button->frame(), -2.0f);

    View::draw(cr);
}

auto TitleBar::draw_placeholder(cairo_t* cr, const gfx::Rect& rect, float dx) -> void
{
    auto placeholder_rect = rect.inset_by(dx, 0.0f);
    cairo_rectangle(cr, placeholder_rect.x, placeholder_rect.y, placeholder_rect.width, placeholder_rect.height);
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_fill(cr);
}
