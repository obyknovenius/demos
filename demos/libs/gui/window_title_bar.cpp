#include "window_title_bar.h"

#include <Gfx/Color.h>
#include <Gfx/Context.h>

namespace gui
{
    void window::title_bar::redraw(NonnullRefPtr<Gfx::Context> context)
    {
        context->fillRect(m_bounds, Gfx::Color::white);

        int padding = 4;
        int y = 6;
        for (int i = 0; i < 6; ++i, y += 4)
            context->strokeLine({ padding, y }, { m_bounds.size.width - padding, y }, Gfx::Color::black, 2.0f);
        y += 1;
        context->strokeLine({ padding, y }, { m_bounds.size.width - padding, y }, Gfx::Color::black, 2.0f);
    }
}
