#include "window.h"

#include "event.h"
#include "window_decoration_view.h"
#include <gfx/color.h>
#include <gfx/context.h>
#include <gfx/rect.h>

namespace gui
{
    window::window(gfx::size const& size) :
        m_size { size },
        m_decoration_view { decoration_view::create({ 0, 0, size.width, size.height }) }
    {
    }

    window::~window() = default;

    auto window::dispatch_event(std::unique_ptr<const event> event) -> void
    {
        close();
    }

    auto window::close() -> void
    {
        if (on_close)
            on_close();
    }

    auto window::redraw(nonnull_ref_ptr<gfx::context> context) -> void
    {
        context->fill_rect({{0, 0}, {m_size.width, m_size.height}}, gfx::color::white);
        m_decoration_view->redraw(context);
    }
}
