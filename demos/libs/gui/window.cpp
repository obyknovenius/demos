#include "window.h"

#include "event.h"
#include "window_decoration_view.h"
#include "window_title_bar.h"
#include <gfx/color.h>
#include <gfx/context.h>
#include <gfx/rect.h>

namespace gui
{
    window::window(gfx::size const& size) :
        m_size { size },
        m_decoration_view { make_ref_counted<decoration_view>(gfx::rect { 0, 0, size.width, size.height }) }
    {
    }

    window::~window() = default;

    bool window::should_start_move(const gfx::point& pointer_position) const
    {
        if (m_decoration_view->get_title_bar()->get_frame().contains(pointer_position))
            return true;
        return false;
    }

    void window::dispatch_event(std::unique_ptr<const event> event)
    {
        close();
    }

    void window::close()
    {
        if (on_close)
            on_close();
    }

    void window::redraw(nonnull_ref_ptr<gfx::context> context)
    {
        context->fill_rect({0, 0, m_size.width, m_size.height}, gfx::color::white);
        m_decoration_view->redraw(context);
    }
}
