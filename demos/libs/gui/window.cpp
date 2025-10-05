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
    context->fill_rect({{0, 0}, m_size}, gfx::color::white);
    int line_width = 2;
    gfx::point origin = {line_width / 2, line_width / 2};
    gfx::size size = m_size.inset(line_width, line_width);
    context->stroke_rect({origin, size}, gfx::color::black, line_width);
}

}
