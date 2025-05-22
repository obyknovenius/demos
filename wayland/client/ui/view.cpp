#include "view.h"

auto View::layout() -> void
{
    for (const auto& subview : m_subviews) {
        subview->layout();
    }
}

auto View::draw(cairo_t* cr) -> void
{
    for (const auto& subview : m_subviews) {
        cairo_save(cr);
        cairo_translate(cr, subview->frame().x, subview->frame().y);
        subview->draw(cr);
        cairo_restore(cr);
    }
}
