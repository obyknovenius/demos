#include "WindowTitleBar.h"

#include <Gfx/Color.h>
#include <Gfx/Context.h>

namespace GUI
{
    void Window::TitleBar::redraw(NonnullRefPtr<Gfx::Context> context)
    {
        context->fillRect(_bounds, Gfx::Color::white);

        int padding = 4;
        int y = 6;
        for (int i = 0; i < 6; ++i, y += 4)
            context->strokeLine({ padding, y }, { _bounds.size.width - padding, y }, Gfx::Color::black, 2.0f);
        y += 1;
        context->strokeLine({ padding, y }, { _bounds.size.width - padding, y }, Gfx::Color::black, 2.0f);
    }

    void Window::TitleBar::onPointerEntered()
    {
    }

    void Window::TitleBar::onPointerMoved()
    {
    }

    void Window::TitleBar::onPointerLeft()
    {
    }
}
