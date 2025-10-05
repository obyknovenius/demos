#include "window.h"

#include "event.h"
#include <gfx/color.h>
#include <gfx/context.h>
#include <gfx/rect.h>

namespace gui {

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
    m_decoration_view->redraw(context);
}

auto window::decoration_view::redraw(nonnull_ref_ptr<gfx::context> context) -> void
{
    context->fill_rect(m_frame, gfx::color::white);
    int line_width = 2;
    context->stroke_rect(m_frame.inset(line_width / 2), gfx::color::black, line_width);
}

}
